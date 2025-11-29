/*
  GigaSimpleEmail
  Example for EMailSender library: connect to WiFi, send email with attachment
  Attachment is read from a file (works for SD/USB mass storage if mounted as a file)

  IMPORTANT: Replace credentials in a separate header or via build flags. Do not commit
  real credentials into source control.
*/

#include <Arduino.h>
#include <WiFiNINA.h>
#include "EMailSender.h"
#include <SPI.h>
#include <SD.h> // used to read attachment from a removable storage (SPI/USB host should provide filesystem mounted to this API)

// Credentials are intentionally left as placeholders
#ifndef EXAMPLE_SECRETS_H
// Define these via build flags or create an excluded header named ExampleSecrets.h
#define SECRET_SSID "YOUR_SSID"
#define SECRET_PASS "YOUR_WIFI_PASSWORD"
#define EMAIL_LOGIN "your_login@example.com"
#define EMAIL_PASS  "YOUR_EMAIL_PASSWORD"
#define EMAIL_FROM  "your_from@example.com"
#define EMAIL_NAME  "Arduino Giga"
#define SMTP_SERVER "smtp.example.com"
#define SMTP_PORT   465
#define RECIPIENT   "recipient@example.com"
#endif

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Attachment file path on removable storage
const char ATTACH_PATH[] = "/email_attach.txt"; // put this file on the USB stick / SD card

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }
  Serial.println("GigaSimpleEmail example");

  // Connect to WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if ((millis() - start) > 20000) {
      Serial.println("WiFi connection timeout");
      break;
    }
    delay(500);
    Serial.print('.');
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("No WiFi - continue (send will likely fail)");
  }

  // Initialize storage (SD/USB). Adjust chip select or mounting per your hardware.
  if (!SD.begin()) {
    Serial.println("Warning: Storage not mounted (SD.begin failed). Attachment will be skipped.");
  } else {
    Serial.println("Storage mounted");
  }

  // Setup mailer
  EMailSender mailSender(EMAIL_LOGIN, EMAIL_PASS, EMAIL_FROM, EMAIL_NAME, SMTP_SERVER, SMTP_PORT);
  mailSender.setIsSecure(true);

  EMailSender::EMailMessage message;
  message.subject = "Giga: email with attachment from removable storage";
  message.message = "This email contains an attachment read from removable storage (SD/USB).";
  message.mime = String(MIME_TEXT_PLAIN);

  const char* to[] = { RECIPIENT };

  // Try to open attachment
  if (SD.begin() && SD.exists(ATTACH_PATH)) {
    File f = SD.open(ATTACH_PATH, FILE_READ);
    if (f) {
      // Add attachment as stream. The library supports adding attachment from stream
      // by providing a function that reads bytes; here we use File which implements Stream
      EMailSender::Attachment att;
      att.contentType = String("text/plain");
      att.fileName = String("attachment.txt");
      att.fromStream = true; // indicate we'll provide a Stream pointer
      att.stream = &f; // pointer to Stream (File derives from Stream)

      // Note: EMailSender API may differ; adjust if library expects other fields
      mailSender.addAttachment(att);

      Serial.println("Sending email with attachment...");
      EMailSender::Response resp = mailSender.send(to, 1, message);

      Serial.print("Send status: ");
      Serial.println(resp.status ? "OK" : "FAILED");
      Serial.print("Code: ");
      Serial.println(resp.code);
      Serial.print("Desc: ");
      Serial.println(resp.desc);

      f.close();
    } else {
      Serial.println("Failed to open attachment file");
      Serial.println("Sending email without attachment...");
      EMailSender::Response resp = mailSender.send(to, 1, message);
      Serial.print("Send status: ");
      Serial.println(resp.status ? "OK" : "FAILED");
    }
  } else {
    Serial.println("Attachment not found; sending without attachment");
    EMailSender::Response resp = mailSender.send(to, 1, message);
    Serial.print("Send status: ");
    Serial.println(resp.status ? "OK" : "FAILED");
  }
}

void loop() {
  delay(10000);
}

