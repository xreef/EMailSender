/*
 * EMailSender library for Arduino, esp8266 and esp32
 * Simple esp32 Gmail send to a distribution list example
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <EMailSender.h>
#include <WiFi.h>

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("<YOUR-SMTP>", "<YOUR-SMTP-PASSWD>");

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);
        Serial.println(nSSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}

void setup()
{
    Serial.begin(115200);
    const char* ssid = "<YOUR-SSID>";
    const char* password = "<YOUR-PASSWD>";

    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    EMailSender::EMailMessage message;
    message.subject = "Soggetto";
    message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

    // Send to 3 different email
    const char* arrayOfEmail[] = {"<FIRST>@gmail.com", "<SECOND>@yahoo.com", "<THIRD>@hotmail.com"};
    EMailSender::Response resp = emailSend.send(arrayOfEmail, 3, message);

//    // Send to 3 different email, 2 in C and 1 in CC
//    const char* arrayOfEmail[] = {"<FIRST>@gmail.com", "<SECOND>@yahoo.com", "<THIRD>@hotmail.com"};
//    EMailSender::Response resp = emailSend.send(arrayOfEmail, 2, 1, message);
//
//    // Send to 3 different email first to C second to CC and third to CCn
//    const char* arrayOfEmail[] = {"<FIRST>@gmail.com", "<SECOND>@yahoo.com", "<THIRD>@hotmail.com"};
//    EMailSender::Response resp = emailSend.send(arrayOfEmail, 3, message);


    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
}

void loop()
{

}
