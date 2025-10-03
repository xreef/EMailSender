/*
 * EMailSender — Send Email with attachments
 * Example: ESP8266 Gmail over implicit TLS on port 465
 * Board: ESP8266
 * Transport: WiFi (WiFiClientSecure)
 * TLS: Implicit TLS (465)
 * Library: https://github.com/xreef/EMailSender
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org/
 * License: MIT
 * Date: 2025-10-02
 */

#include <ESP8266WiFi.h>
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

  // Gmail: use an App Password (NOT your account password)
  EMailSender emailSend(
    "your.account@gmail.com",   // login (and default from)
    "your-app-password",        // App Password
    "your.account@gmail.com",   // from
    "ESP8266 Test",             // display name
    "smtp.gmail.com",           // SMTP server
    465                          // implicit TLS
  );

  EMailSender::EMailMessage msg;
  msg.subject = "ESP8266 TLS 465";
  msg.message = "Hello from ESP8266 over implicit TLS (465)";

  EMailSender::Response r = emailSend.send("dest@example.com", msg);
  Serial.println(r.code);
  Serial.println(r.desc);
  Serial.println(r.status);
}

void loop() {}
