/*
 * EMailSender — Send Email with attachments
 * Example: ESP32 Gmail over STARTTLS on port 587
 * Board: ESP32
 * Transport: WiFi (WiFiClient)
 * TLS: STARTTLS (587) via internal backend
 * Library: https://github.com/xreef/EMailSender
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org/
 * License: MIT
 * Date: 2025-10-02
 *
 * Prerequisites:
 * - Enable in EMailSenderKey.h the internal STARTTLS backend:
 *     #define EMAIL_ENABLE_INTERNAL_SSLCLIENT
 *   (or add the compile flag: -DEMAIL_ENABLE_INTERNAL_SSLCLIENT)
 */

#include <WiFi.h>
#include <EMailSender.h>

// Replace with your WiFi credentials
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(200); Serial.print('.'); }
  Serial.println();
  Serial.println("WiFi connected");

  // Gmail: use an App Password
  EMailSender emailSend(
    "your.account@gmail.com",   // login (and default from)
    "your-app-password",        // App Password (NOT your account password)
    "your.account@gmail.com",   // from
    "ESP32 Test",               // display name
    "smtp.gmail.com",           // SMTP server
    587                          // STARTTLS
  );
  emailSend.setClientType(EMailSender::CLIENT_SSLCLIENT); // enable internal STARTTLS backend (ESP32)

  EMailSender::EMailMessage msg;
  msg.subject = "ESP32 STARTTLS test";
  msg.message = "Hello from ESP32 over STARTTLS (587)";

  EMailSender::Response r = emailSend.send("dest@example.com", msg);
  Serial.println(r.code);
  Serial.println(r.desc);
  Serial.println(r.status);
}

void loop() {}
