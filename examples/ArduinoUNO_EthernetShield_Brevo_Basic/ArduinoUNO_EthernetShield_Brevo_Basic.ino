/*
 * EMailSender Library - Arduino UNO + Ethernet Shield + Brevo Basic Email
 *
 * This example shows how to send simple emails from Arduino UNO using minimal RAM.
 * Perfect for memory-constrained projects that need email notifications.
 *
 * MEMORY USAGE:
 * - RAM:   [======    ]  58.1% (used 1190 bytes from 2048 bytes)
 * - Flash: [=======   ]  70.8% (used 22832 bytes from 32256 bytes)
 * - No attachments to minimize memory usage (comment #define ENABLE_ATTACHMENTS in EMailSenderKey.h)
 *
 * WHY BREVO FOR ARDUINO UNO?
 * - No SSL/TLS required (UNO has only 2KB RAM)
 * - Port 587 with simple AUTH LOGIN
 * - 300 emails/day free tier
 *
 * HARDWARE:
 * - Arduino UNO R3
 * - W5100/W5500 Ethernet Shield
 * - Ethernet cable
 *
 * WIRING:
 * - Stack the shield on top of Arduino UNO
 * - MOSI → Pin 11, MISO → Pin 12, SCK → Pin 13, CS → Pin 10
 *
 * SETUP:
 * 1. Register at https://www.brevo.com
 * 2. Get SMTP credentials from SMTP & API settings
 * 3. Verify sender email address in Brevo dashboard
 * 4. Update credentials below
 *
 * Author: Renzo Mischianti
 * Website: https://www.mischianti.org
 * Library: https://github.com/xreef/EMailSender
 */

#include <SPI.h>
#include <Ethernet.h>
#include <EMailSender.h>

// ========== CONFIGURATION ==========
const char* BREVO_LOGIN = "your-login@smtp-brevo.com";
const char* BREVO_PASSWORD = "xsmtpsib-YOUR-API-KEY";
const char* EMAIL_FROM = "your-verified@example.com";
const char* EMAIL_TO = "recipient@example.com";
const char* SENDER_NAME = "ArduinoUNO";

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Optional: Static IP (uncomment if needed)
// IPAddress ip(192, 168, 1, 177);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

// ========== GLOBALS ==========
EMailSender emailSend(BREVO_LOGIN, BREVO_PASSWORD, EMAIL_FROM, SENDER_NAME, "smtp-relay.brevo.com", 587);

// ========== FUNCTIONS ==========
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

bool initializeEthernet() {
  Serial.println(F("Initializing Ethernet..."));

  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("DHCP failed!"));
    #ifdef ip
    Ethernet.begin(mac, ip, gateway, subnet);
    #else
    return false;
    #endif
  }

  delay(1500);

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println(F("No Ethernet cable!"));
    return false;
  }

  Serial.print(F("IP: "));
  Serial.println(Ethernet.localIP());
  return true;
}

void sendEmail() {
  Serial.println(F("\n--- Sending Email ---"));
  Serial.print(F("Free RAM: "));
  Serial.print(freeRam());
  Serial.println(F(" bytes"));

  EMailSender::EMailMessage message;
  message.subject = F("Arduino UNO + Brevo Test");
  message.message = F(" Hello from Arduino UNO!\n\n");
  message.message += F("This email was sent using:\n");
  message.message += F("- Arduino UNO R3\n");
  message.message += F("- Ethernet Shield\n");
  message.message += F("- Brevo SMTP (port 587)\n");
  message.message += F("- EMailSender Library\n\n");
  message.message += F("System Info:\n- Free RAM: ");
  message.message += String(freeRam());
  message.message += F(" bytes\n- Uptime: ");
  message.message += String(millis() / 1000);
  message.message += F(" sec\n\n");
  message.message += F("---\nby Renzo Mischianti\n");
  message.message += F("mischianti.org\n");
  message.mime = MIME_TEXT_PLAIN;

  Serial.print(F("From: "));
  Serial.println(EMAIL_FROM);
  Serial.print(F("To: "));
  Serial.println(EMAIL_TO);
  Serial.println(F("Connecting to Brevo..."));

  EMailSender::Response resp = emailSend.send(EMAIL_TO, message);

  Serial.println(F("\n--- Result ---"));
  Serial.print(F("Code: "));
  Serial.println(resp.code);
  Serial.print(F("Status: "));
  Serial.println(resp.status ? F("✓ OK") : F("✗ FAILED"));

  if (!resp.status) {
    Serial.print(F("Error: "));
    Serial.println(resp.desc);
  }
}

// ========== MAIN ==========
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println(F("\n=============================="));
  Serial.println(F("Arduino UNO + Brevo"));
  Serial.println(F("==============================\n"));
  Serial.print(F("Free RAM: "));
  Serial.print(freeRam());
  Serial.println(F(" bytes"));

  if (!initializeEthernet()) {
    Serial.println(F("Network init failed!"));
    while (1) {
      delay(5000);
      if (initializeEthernet()) break;
    }
  }

  Serial.println(F("✓ Network ready!\n"));
  sendEmail();
  Serial.println(F("\n=== Setup Complete ==="));
}

void loop() {
  Ethernet.maintain();
  delay(10000);
}
