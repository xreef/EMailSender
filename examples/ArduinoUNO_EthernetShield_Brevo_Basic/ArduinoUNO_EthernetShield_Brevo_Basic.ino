/*
 * EMailSender Library - Arduino UNO + Ethernet Shield + Brevo Basic Email
 *
 * This example shows how to send simple emails from Arduino UNO using minimal RAM.
 * Perfect for memory-constrained projects that need email notifications.
 *
 * MEMORY USAGE:
 * - RAM: ~1254 bytes (61% of 2048 bytes available)
 * - Flash: ~20KB (64% of 32KB available)
 * - No attachments to minimize memory usage comment the #define ENABLE_ATTACHMENTS in EMailSenderKey.h
 * - Optimized string handling to avoid heap fragmentation
 *
 * WHY BREVO FOR ARDUINO UNO?
 * - No SSL/TLS required (UNO has only 2KB RAM)
 * - Port 587 with simple AUTH LOGIN
 * - Lightweight SMTP protocol implementation
 * - 300 emails/day free tier
 * - No domain verification needed
 *
 * HARDWARE REQUIREMENTS:
 * - Arduino UNO R3
 * - W5100/W5500 Ethernet Shield
 * - Ethernet cable
 * - Stable 5V power supply
 *
 * MEMORY OPTIMIZATION TECHNIQUES:
 * - Use F() macro for string literals
 * - Minimize String object usage
 * - Reuse variables where possible
 * - Short email messages
 * - No attachments or base64 encoding
 *
 * SETUP INSTRUCTIONS:
 * 1. Register at https://www.brevo.com (free)
 * 2. Get SMTP credentials from SMTP & API settings
 * 3. Verify sender email address
 * 4. Update credentials below
 * 5. Upload and test!
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

// Brevo SMTP Credentials
const char* BREVO_LOGIN = "98a01e002@smtp-brevo.com";        // Your Brevo SMTP login
const char* BREVO_PASSWORD = "xsmtpsib-YOUR-API-KEY-HERE";   // Your Brevo API key

// Email settings
const char* EMAIL_FROM = "your-verified@example.com";        // Verified in Brevo
const char* EMAIL_TO = "recipient@example.com";              // Recipient
const char* SENDER_NAME = "ArduinoUNO";                     // Short sender name

// Network settings
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};        // Unique MAC address

// Optional static IP (comment out for DHCP)
// IPAddress ip(192, 168, 1, 177);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

// ========== END CONFIGURATION ==========

// Global EMailSender instance (minimize stack usage)
EMailSender emailSend(
  BREVO_LOGIN,
  BREVO_PASSWORD,
  EMAIL_FROM,
  SENDER_NAME,
  "smtp-relay.brevo.com",
  587
);

// Email counter for testing
uint16_t emailCount = 0;

// ========== MEMORY MONITORING ==========

int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void printMemoryStats() {
  Serial.print(F("Free RAM: "));
  Serial.print(freeRam());
  Serial.println(F(" bytes"));
}

// ========== NETWORK FUNCTIONS ==========

bool initializeEthernet() {
  Serial.println(F("Initializing Ethernet..."));

  // Try DHCP first
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("DHCP failed!"));

    // Fallback to static IP if configured
    #ifdef ip
    Serial.println(F("Using static IP..."));
    Ethernet.begin(mac, ip, gateway, subnet);
    #else
    Serial.println(F("No static IP configured. Check network!"));
    return false;
    #endif
  }

  // Wait for link
  delay(1500);

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("No Ethernet cable!"));
    return false;
  }

  // Print network info
  Serial.print(F("IP: "));
  Serial.println(Ethernet.localIP());

  return true;
}

// ========== EMAIL FUNCTIONS ==========

bool sendBasicEmail() {
  Serial.println(F("\n--- Sending Email ---"));
  printMemoryStats();

  // Prepare message (use minimal memory)
  EMailSender::EMailMessage msg;
  msg.subject = F("Arduino UNO Status Report");

  // Build message efficiently (avoid String concatenation)
  msg.message = F(" Status: OK\n");  // Space prevents first char bug
  msg.message += F("Device: Arduino UNO\n");
  msg.message += F("Email #: ");
  msg.message += String(++emailCount);
  msg.message += F("\nUptime: ");
  msg.message += String(millis() / 1000);
  msg.message += F(" sec\nRAM: ");
  msg.message += String(freeRam());
  msg.message += F(" bytes\n\nSent via EMailSender + Brevo\n");
  msg.message += F("Library: https://mischianti.org\n");
  msg.message += F("Author: Renzo Mischianti");

  msg.mime = MIME_TEXT_PLAIN;

  Serial.print(F("To: "));
  Serial.println(EMAIL_TO);
  Serial.print(F("Subject: "));
  Serial.println(msg.subject);

  printMemoryStats();

  // Send email
  Serial.println(F("Connecting to Brevo..."));
  EMailSender::Response resp = emailSend.send(EMAIL_TO, msg);

  // Check result
  Serial.print(F("Result: "));
  if (resp.status) {
    Serial.println(F("✅ OK!"));
    Serial.println(F("Message sent!"));
    return true;
  } else {
    Serial.println(F("❌ FAILED"));
    Serial.print(F("Code: "));
    Serial.println(resp.code);
    Serial.print(F("Error: "));
    Serial.println(resp.desc);
    return false;
  }
}

bool sendSensorAlert(float temperature, float humidity) {
  Serial.println(F("\n--- Sending Sensor Alert ---"));

  EMailSender::EMailMessage msg;
  msg.subject = F("Arduino UNO Sensor Alert");

  // Efficient message building
  msg.message = F(" SENSOR ALERT!\n\n");
  msg.message += F("Temperature: ");
  msg.message += String(temperature, 1);
  msg.message += F("°C\nHumidity: ");
  msg.message += String(humidity, 1);
  msg.message += F("%\n\nTimestamp: ");
  msg.message += String(millis());
  msg.message += F(" ms\nDevice: Arduino UNO\nLocation: Lab\n\n");
  msg.message += F("This is an automated alert.\n\n");
  msg.message += F("Library: https://mischianti.org\n");
  msg.message += F("Author: Renzo Mischianti");

  msg.mime = MIME_TEXT_PLAIN;

  Serial.print(F("Temp: "));
  Serial.print(temperature);
  Serial.print(F("°C, Humidity: "));
  Serial.print(humidity);
  Serial.println(F("%"));

  EMailSender::Response resp = emailSend.send(EMAIL_TO, msg);

  if (resp.status) {
    Serial.println(F("✅ Alert sent!"));
    return true;
  } else {
    Serial.println(F("❌ Alert failed!"));
    return false;
  }
}

// ========== MAIN PROGRAM ==========

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for serial connection
  }

  Serial.println(F("\n=============================="));
  Serial.println(F("Arduino UNO Email Sender"));
  Serial.println(F("Memory Optimized Version"));
  Serial.println(F("==============================\n"));

  printMemoryStats();

  // Initialize network
  if (!initializeEthernet()) {
    Serial.println(F("Network init failed!"));
    Serial.println(F("Check cables and settings."));
    while (1) {
      delay(5000);
      Serial.println(F("Retrying network..."));
      if (initializeEthernet()) break;
    }
  }

  Serial.println(F("✅ Network ready!"));
  printMemoryStats();

  // Send startup email
  Serial.println(F("Sending startup notification..."));
  sendBasicEmail();

  Serial.println(F("\n=== Setup Complete ==="));
  printMemoryStats();
}

void loop() {
  // Keep Ethernet connection alive
  Ethernet.maintain();

  // Nothing else to do - email was sent once in setup()
  delay(10000);
}
