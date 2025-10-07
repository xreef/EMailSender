/*
 * EMailSender Library - Arduino Mega + Brevo + SD Attachments Example
 *
 * This example shows how to send email with SD card attachments using Arduino Mega
 * with Ethernet shield and Brevo SMTP service (port 587 without SSL/TLS).
 *
 * WHY BREVO?
 * - Arduino Mega has only 8KB RAM (not enough for SSL/TLS which needs 24KB+)
 * - Brevo accepts port 587 WITHOUT SSL/TLS encryption
 * - 300 emails/day free tier
 * - No domain verification required (just verify your email address)
 *
 * SD CARD SETUP:
 * - SD card shares SPI bus with Ethernet
 * - Typical CS pins: Ethernet=10, SD=4
 * - Files are created automatically if they don't exist
 * - Supports multiple attachments from SD
 *
 * SETUP INSTRUCTIONS:
 * 1. Register at https://www.brevo.com (free account)
 * 2. Go to SMTP & API → SMTP settings
 * 3. Get your SMTP credentials:
 *    - Login: something like "98a01e002@smtp-brevo.com"
 *    - Password: your API key (e.g., "xsmtpsib-abc123...")
 * 4. Verify a sender email address in Brevo dashboard
 * 5. Insert SD card and update the credentials below
 *
 * HARDWARE:
 * - Arduino Mega 2560
 * - W5500 or W5100 Ethernet Shield with SD slot
 * - SD card (formatted FAT16/FAT32)
 * - Ethernet cable connected to router
 *
 * WIRING (W5500 + SD):
 * - MOSI → Pin 51
 * - MISO → Pin 50
 * - SCK  → Pin 52
 * - ETH CS → Pin 10
 * - SD CS  → Pin 4
 * - VCC  → 5V
 * - GND  → GND
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 * Library: https://github.com/xreef/EMailSender
 * License: MIT
 * Date: 2025-01-07
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <EMailSender.h>

// ========== CONFIGURATION - UPDATE THESE VALUES ==========

// Brevo SMTP Credentials (get from https://app.brevo.com/settings/keys/smtp)
const char* BREVO_LOGIN = "98a01e002@smtp-brevo.com";        // Your Brevo SMTP login
const char* BREVO_PASSWORD = "xsmtpsib-YOUR-API-KEY-HERE";   // Your Brevo SMTP password/API key

// Email addresses
const char* EMAIL_FROM = "your-verified-email@example.com";   // Must be verified in Brevo
const char* EMAIL_TO = "recipient@example.com";               // Recipient email

// Ethernet MAC address (make it unique on your network)
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// SPI Chip Select pins
const uint8_t ETH_CS = 10;  // Ethernet chip select
const uint8_t SD_CS = 4;    // SD card chip select

// Optional: Static IP configuration (comment out to use DHCP)
// IPAddress ip(192, 168, 1, 177);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress dns(8, 8, 8, 8);

// ========== END CONFIGURATION ==========

// Create EMailSender instance for Brevo
EMailSender emailSend(
  BREVO_LOGIN,              // Brevo SMTP login
  BREVO_PASSWORD,           // Brevo SMTP password
  EMAIL_FROM,               // From email (must be verified in Brevo)
  "ArduinoMega",            // Sender name
  "smtp-relay.brevo.com",   // Brevo SMTP server
  587                        // Port 587 (AUTH LOGIN without SSL)
);

// ========== SD CARD UTILITIES ==========

bool writeTextFile(const char* path, const char* content) {
  File file = SD.open(path, FILE_WRITE);
  if (!file) {
    Serial.print("Failed to create file: ");
    Serial.println(path);
    return false;
  }

  file.print(content);
  file.close();

  Serial.print("Created file: ");
  Serial.print(path);
  Serial.print(" (");
  Serial.print(strlen(content));
  Serial.println(" bytes)");

  return true;
}

bool ensureFileExists(const char* path, const char* defaultContent) {
  if (SD.exists(path)) {
    Serial.print("File exists: ");
    Serial.println(path);
    return true;
  }

  Serial.print("Creating file: ");
  Serial.println(path);
  return writeTextFile(path, defaultContent);
}

void listSDFiles() {
  Serial.println("\n--- SD Card Contents ---");
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open root directory");
    return;
  }

  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;

    Serial.print("File: ");
    Serial.print(entry.name());
    Serial.print(" (");
    Serial.print(entry.size());
    Serial.println(" bytes)");

    entry.close();
  }
  root.close();
  Serial.println("--- End SD Contents ---\n");
}

// ========== MAIN FUNCTIONS ==========

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }

  Serial.println("\n========================================");
  Serial.println("Arduino Mega + Brevo + SD Attachments");
  Serial.println("========================================\n");

  // Initialize SPI pins (deselect all devices first)
  pinMode(ETH_CS, OUTPUT);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(ETH_CS, HIGH);
  digitalWrite(SD_CS, HIGH);

  // Initialize SD card first
  Serial.print("Initializing SD card (CS=");
  Serial.print(SD_CS);
  Serial.println(")...");

  digitalWrite(ETH_CS, HIGH);  // Ensure Ethernet is deselected
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR: SD card initialization failed!");
    Serial.println("Check:");
    Serial.println("  - SD card is inserted");
    Serial.println("  - SD card is formatted (FAT16/FAT32)");
    Serial.println("  - Wiring is correct");
    Serial.println("  - CS pin is correct");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("✓ SD card initialized!");

  // Create sample files for attachments
  Serial.println("\nPreparing sample files...");

  String logContent = "Arduino Mega Email Log\r\n";
  logContent += "======================\r\n";
  logContent += "Timestamp: " + String(millis()) + " ms\r\n";
  logContent += "Free RAM: " + String(freeRam()) + " bytes\r\n";
  logContent += "Uptime: " + String(millis() / 1000) + " seconds\r\n";
  logContent += "\r\nThis file was created automatically\r\n";
  logContent += "and sent as an email attachment.\r\n";

  String csvContent = "timestamp,sensor_id,value,unit\r\n";
  csvContent += "0,temp_01,23.5,C\r\n";
  csvContent += "1000,temp_01,24.1,C\r\n";
  csvContent += "2000,temp_01,23.8,C\r\n";
  csvContent += "3000,hum_01,65.2,%\r\n";
  csvContent += "4000,hum_01,66.8,%\r\n";
  csvContent += "5000,hum_01,64.5,%\r\n";

  ensureFileExists("/system.log", logContent.c_str());
  ensureFileExists("/sensors.csv", csvContent.c_str());

  listSDFiles();

  // Initialize Ethernet
  Serial.println("Initializing Ethernet...");
  digitalWrite(SD_CS, HIGH);  // Ensure SD is deselected

  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP failed!");

    // Fallback to static IP (uncomment the IP variables above)
    // Serial.println("Trying static IP...");
    // Ethernet.begin(mac, ip, dns, gateway, subnet);

    Serial.println("ERROR: Cannot initialize Ethernet!");
    Serial.println("Check:");
    Serial.println("  - Ethernet cable is connected");
    Serial.println("  - Shield is properly seated");
    Serial.println("  - Router DHCP is enabled");
    while (1) {
      delay(1000);
    }
  }

  // Give Ethernet time to initialize
  delay(1500);

  // Print network information
  Serial.println("✓ Ethernet connected!");
  Serial.print("  IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.print("  Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("  Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("  DNS Server: ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println();

  // Check link status
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("WARNING: Ethernet cable not connected!");
  }

  // Send test email with attachments
  Serial.println("Preparing to send email with SD attachments...");
  sendEmailWithAttachments();
}

void loop() {
  // Keep Ethernet connection alive
  Ethernet.maintain();

  // Nothing else to do in loop
  delay(10000);
}

void sendEmailWithAttachments() {
  Serial.println("\n--- Sending Email with SD Attachments ---");

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Arduino Mega + Brevo + SD Attachments";

  // IMPORTANT: Add space at beginning to avoid first character bug
  message.message = " Hello from Arduino Mega with SD attachments!\n\n";
  message.message += "This email was sent using:\n";
  message.message += "- Arduino Mega 2560\n";
  message.message += "- Ethernet Shield (W5500/W5100)\n";
  message.message += "- SD Card for file storage\n";
  message.message += "- Brevo SMTP service (port 587 without SSL)\n";
  message.message += "- EMailSender Library v4.0.0\n\n";
  message.message += "Attached files from SD card:\n";
  message.message += "✓ system.log - System log file\n";
  message.message += "✓ sensors.csv - Sensor data in CSV format\n\n";
  message.message += "System Info:\n";
  message.message += "- Free RAM: " + String(freeRam()) + " bytes\n";
  message.message += "- Uptime: " + String(millis() / 1000) + " seconds\n";
  message.message += "- Timestamp: " + String(millis()) + " ms\n\n";
  message.message += "Brevo is perfect for Arduino because:\n";
  message.message += "✓ No SSL/TLS required (Arduino Mega has only 8KB RAM)\n";
  message.message += "✓ 300 emails/day free tier\n";
  message.message += "✓ No domain verification needed\n";
  message.message += "✓ Simple AUTH LOGIN authentication\n\n";
  message.message += "---\n";
  message.message += "Sent using EMailSender Library by Renzo Mischianti\n";
  message.message += "More info: https://www.mischianti.org\n";
  message.message += "GitHub: https://github.com/xreef/EMailSender\n";

  message.mime = MIME_TEXT_PLAIN;

  // Prepare attachments from SD card
  EMailSender::FileDescriptior fileDescriptor[2];

  // First attachment: system.log
  fileDescriptor[0].filename = F("system.log");
  fileDescriptor[0].url = F("/system.log");
  fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;
  fileDescriptor[0].mime = MIME_TEXT_PLAIN;
  // fileDescriptor[0].encode64 = true; // Optional: base64 encoding

  // Second attachment: sensors.csv
  fileDescriptor[1].filename = F("sensors.csv");
  fileDescriptor[1].url = F("/sensors.csv");
  fileDescriptor[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;
  fileDescriptor[1].mime = "text/csv";
  // fileDescriptor[1].encode64 = true; // Optional: base64 encoding

  EMailSender::Attachments attachments = {2, fileDescriptor};

  // Print email details
  Serial.println("From: " + String(EMAIL_FROM));
  Serial.println("To: " + String(EMAIL_TO));
  Serial.println("Subject: " + message.subject);
  Serial.println("Attachments: 2 files from SD card");
  Serial.println("  - system.log (text/plain)");
  Serial.println("  - sensors.csv (text/csv)");
  Serial.println();

  // Send email
  Serial.println("Connecting to Brevo SMTP server...");
  Serial.println("Server: smtp-relay.brevo.com:587");

  EMailSender::Response resp = emailSend.send(EMAIL_TO, message, attachments);

  // Print result
  Serial.println("\n--- Result ---");
  Serial.print("Status Code: ");
  Serial.println(resp.code);
  Serial.print("Description: ");
  Serial.println(resp.desc);
  Serial.print("Success: ");
  Serial.println(resp.status ? "YES ✓" : "NO ✗");

  if (resp.status) {
    Serial.println("\n✓✓✓ EMAIL WITH ATTACHMENTS SENT SUCCESSFULLY! ✓✓✓");
    Serial.println("Check your inbox at: " + String(EMAIL_TO));
    Serial.println("The email should contain 2 attachments from SD card.");
  } else {
    Serial.println("\n✗✗✗ EMAIL SENDING FAILED! ✗✗✗");
    Serial.println("\nTroubleshooting:");
    Serial.println("1. Check Brevo credentials are correct");
    Serial.println("2. Verify sender email in Brevo dashboard");
    Serial.println("3. Check internet connection");
    Serial.println("4. Make sure port 587 is not blocked by firewall");
    Serial.println("5. Verify Brevo account is active");
    Serial.println("6. Check SD card files exist and are readable");
    Serial.println("7. Ensure enough free RAM for attachments");
  }

  Serial.println("\n========================================\n");
}

// Utility function to check free RAM
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
