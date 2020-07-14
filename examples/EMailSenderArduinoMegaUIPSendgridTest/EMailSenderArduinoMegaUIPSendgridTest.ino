/*
 * EMailSender library for Arduino, esp8266 and esp32
 * Arduino Mega and UIPEthernet send example with Sendgrid provider
 *
 * Pay attention you must set in the library
 * #define DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO 	NETWORK_ENC28J60
 * for UIPEthernet
 *
 * #define DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO 	NETWORK_W5100
 * for standard Ethernet
 *
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <SPI.h>
#include <UIPEthernet.h>

#include <EMailSender.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EMailSender emailSend("<YOUR-SENDGRID-API-KEY>", "<YOUR-SENDGRID-PASSWD>", "<FROM-EMAIL>", "smtp.sendgrid.net", 25);

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
//    while (!Serial) {}

    delay(2000);

    Serial.println("Starting!");

    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      while(1);
    }
    Serial.print("IP address ");
    Serial.println(Ethernet.localIP());


    EMailSender::EMailMessage message;
    message.subject = "Soggetto";
    message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

    EMailSender::Response resp = emailSend.send("email_to_receive@gmail.com", message);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
}

void loop()
{

}
