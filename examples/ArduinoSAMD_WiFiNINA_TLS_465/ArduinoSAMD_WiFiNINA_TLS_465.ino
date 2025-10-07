/*
 * EMailSender — Send Email with attachments
 * Example: Arduino SAMD + WiFiNINA Gmail over implicit TLS on port 465
 * Boards: MKR WiFi 1010, UNO WiFi Rev2, etc.
 * Transport: WiFi (WiFiSSLClient)
 * TLS: Implicit TLS (465)
 * Library: https://github.com/xreef/EMailSender
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org/
 * License: MIT
 * Date: 2025-10-02
 */

#include <WiFiNINA.h>
#include <EMailSender.h>

// Replace with your WiFi credentials
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASS";

void setup(){
  Serial.begin(115200);
  while (!Serial) {}

  int status = WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (status != WL_CONNECTED) { delay(200); Serial.print('.'); status = WiFi.status(); }
  Serial.println();
  Serial.println("WiFi connected");

  // Gmail: use an App Password (NOT your account password)
  EMailSender emailSend(
    "your.account@gmail.com",   // login (and default from)
    "your-app-password",        // App Password
    "your.account@gmail.com",   // from
    "SAMD WiFiNINA",            // display name
    "smtp.gmail.com",           // SMTP server
    465                          // implicit TLS
  );

  EMailSender::EMailMessage msg;
  msg.subject = "SAMD WiFiNINA TLS 465";
  msg.message = "Hello from SAMD + WiFiNINA over implicit TLS (465)";

  EMailSender::Response r = emailSend.send("dest@example.com", msg);
  Serial.println(r.code);
  Serial.println(r.desc);
  Serial.println(r.status);
}

void loop(){}
