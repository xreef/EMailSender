/*
 * EMailSender Library - Arduino Mega + Brevo (Sendinblue) Example
 *
 * This example shows how to send email using Arduino Mega with Ethernet shield
 * and Brevo SMTP service (port 587 without SSL/TLS).
 *
 * WHY BREVO?
 * - Arduino Mega has only 8KB RAM (not enough for SSL/TLS which needs 24KB+)
 * - Brevo accepts port 587 WITHOUT SSL/TLS encryption
 * - 300 emails/day free tier
 * - No domain verification required (just verify your email address)
 *
 * SETUP INSTRUCTIONS:
 * 1. Register at https://www.brevo.com (free account)
 * 2. Go to SMTP & API → SMTP settings
 * 3. Get your SMTP credentials:
 *    - Login: something like "98a01e002@smtp-brevo.com"
 *    - Password: your API key (e.g., "xsmtpsib-abc123...")
 * 4. Verify a sender email address in Brevo dashboard
 * 5. Update the credentials below
 *
 * HARDWARE:
 * - Arduino Mega 2560
 * - W5500 or W5100 Ethernet Shield
 * - Ethernet cable connected to router
 *
 * WIRING (W5500):
 * - MOSI → Pin 51
 * - MISO → Pin 50
 * - SCK  → Pin 52
 * - CS   → Pin 10
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

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial port to connect (needed for native USB)
  }

  Serial.println("\n========================================");
  Serial.println("Arduino Mega + Brevo Email Example");
  Serial.println("========================================\n");

  // Initialize Ethernet
  Serial.println("Initializing Ethernet...");

  // Try DHCP first
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

  // Send test email
  Serial.println("Preparing to send email...");
  sendTestEmail();
}

void loop() {
  // Keep Ethernet connection alive
  Ethernet.maintain();

  // Nothing else to do in loop
  delay(10000);
}

void sendTestEmail() {
  Serial.println("\n--- Sending Email ---");

  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Test from Arduino Mega + Brevo";

  // IMPORTANT: Add space at beginning to avoid first character bug
  message.message = " Hello from Arduino Mega!\n\n";
  message.message += "This email was sent using:\n";
  message.message += "- Arduino Mega 2560\n";
  message.message += "- Ethernet Shield (W5500/W5100)\n";
  message.message += "- Brevo SMTP service (port 587 without SSL)\n";
  message.message += "- EMailSender Library v4.0.0\n\n";
  message.message += "Brevo is perfect for Arduino because:\n";
  message.message += "✓ No SSL/TLS required (Arduino Mega has only 8KB RAM)\n";
  message.message += "✓ 300 emails/day free tier\n";
  message.message += "✓ No domain verification needed\n";
  message.message += "✓ Simple AUTH LOGIN authentication\n\n";
  message.message += "Timestamp: " + String(millis() / 1000) + " seconds\n";
  message.message += "\nSent from Arduino Mega 2560";

  message.mime = MIME_TEXT_PLAIN;

  // Print email details
  Serial.println("From: " + String(EMAIL_FROM));
  Serial.println("To: " + String(EMAIL_TO));
  Serial.println("Subject: " + message.subject);
  Serial.println();

  // Send email
  Serial.println("Connecting to Brevo SMTP server...");
  Serial.println("Server: smtp-relay.brevo.com:587");

  EMailSender::Response resp = emailSend.send(EMAIL_TO, message);

  // Print result
  Serial.println("\n--- Result ---");
  Serial.print("Status Code: ");
  Serial.println(resp.code);
  Serial.print("Description: ");
  Serial.println(resp.desc);
  Serial.print("Success: ");
  Serial.println(resp.status ? "YES ✓" : "NO ✗");

  if (resp.status) {
    Serial.println("\n✓✓✓ EMAIL SENT SUCCESSFULLY! ✓✓✓");
    Serial.println("Check your inbox at: " + String(EMAIL_TO));
  } else {
    Serial.println("\n✗✗✗ EMAIL SENDING FAILED! ✗✗✗");
    Serial.println("\nTroubleshooting:");
    Serial.println("1. Check Brevo credentials are correct");
    Serial.println("2. Verify sender email in Brevo dashboard");
    Serial.println("3. Check internet connection");
    Serial.println("4. Make sure port 587 is not blocked by firewall");
    Serial.println("5. Verify Brevo account is active");
  }

  Serial.println("\n========================================\n");
}

