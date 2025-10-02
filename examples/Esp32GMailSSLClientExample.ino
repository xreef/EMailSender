// Esempio di invio email tramite GMail SMTP con EMailSender e SSLClient (ESP_SSLClient)
// Funziona sia su porta 465 (SSL/TLS diretto) che su 587 (STARTTLS)
#include <WiFi.h>
#include "EMailSender.h"
#include "sslclient/CertStore.h"
#include "sslclient/SSLClient.h"

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Parametri GMail (usa una App Password!)
const char* email_login = "tuoindirizzo@gmail.com";
const char* email_password = "APP_PASSWORD_GMAIL";
const char* email_from = "tuoindirizzo@gmail.com";
const char* name_from = "ESP32 Test";
const char* smtp_server = "smtp.gmail.com";
uint16_t smtp_port = 587; // puoi usare anche 465

EMailSender emailSender(email_login, email_password, email_from, name_from, smtp_server, smtp_port);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connesso!");

  // Usa il client avanzato SSLClient (ESP_SSLClient)
  emailSender.setClientType(EMailSender::CLIENT_SSLCLIENT);

  // (Opzionale) Configura trust anchors custom se necessario
  // sslclient::CertStore certStore;
  // certStore.addCert("-----BEGIN CERTIFICATE-----...-----END CERTIFICATE-----");
  // emailSender.sslClient->setTrustAnchors(&certStore);

  // Prepara il messaggio
  EMailSender::EMailMessage message;
  message.subject = "Test invio da ESP32 via GMail";
  message.message = "<b>Funziona!</b> Email inviata da ESP32 usando SSLClient.";
  message.mime = "text/html";

  // Invio email
  EMailSender::Response resp = emailSender.send("destinatario@gmail.com", message);
  Serial.print("Risposta invio: ");
  Serial.println(resp.status ? "OK" : "ERRORE");
  Serial.println(resp.desc);
}

void loop() {
  // Niente
}

