#include <SPI.h>
#include <Ethernet.h>
#include <EMailSender.h>

// IMPORTANT:
// In EMailSenderKey.h enable the OPEnSLab backend:
//   #define EMAIL_ENABLE_OPENSLAB_SSLCLIENT
// Legacy alias also supported:
//   #define EMAIL_ENABLE_EXTERNAL_SSLCLIENT_OPENSLAB
// Then use port 465 (implicit TLS). STARTTLS (587) is not supported by this backend.

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  // Bring up Ethernet via DHCP
  Serial.println("Bringing up Ethernet (DHCP)...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP failed, check cable/router or set a static IP");
    // Example static fallback (uncomment and adjust if you need)
    // IPAddress ip(192,168,1,77);
    // IPAddress dns(192,168,1,1);
    // IPAddress gw(192,168,1,1);
    // IPAddress mask(255,255,255,0);
    // Ethernet.begin(mac, ip, dns, gw, mask);
  }
  delay(1500);
  Serial.print("Local IP: "); Serial.println(Ethernet.localIP());

  // Gmail: requires an App Password (NOT your account password)
  EMailSender emailSend("<YOUR-SMTP>", "<YOUR-SMTP-PASSWD>");

  EMailSender::EMailMessage msg;
  msg.subject = "Mega W5500 TLS 465";
  msg.message = "Hello from Arduino Mega + W5500 via OPEnSLab SSLClient (implicit TLS 465)";

  EMailSender::Response r = emailSend.send("dest@example.com", msg);
  Serial.println(r.code);
  Serial.println(r.desc);
  Serial.println(r.status);
}

void loop() {}
