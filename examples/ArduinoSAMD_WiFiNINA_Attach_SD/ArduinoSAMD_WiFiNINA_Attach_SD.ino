/*
 * EMailSender library for Arduino, Arduino SAMD boards (WiFiNINA), esp8266 and esp32
 * Simple Arduino SAMD boards (WiFiNINA) Gmail send attachments
 * to a single email
 *
 * The base64 encoding of the image is slow, so be patient
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <EMailSender.h>
#include <WiFiNINA.h>

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

const char* ssid = "<YOUR_SSID>";
const char* password = "<YOUR_PASSWD>";

EMailSender emailSend("<smtp_account@gmail.com>", "<PASSWORD>");

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

    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    EMailSender::EMailMessage message;
    message.subject = "Soggetto";
    message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

    EMailSender::FileDescriptior fileDescriptor[2];
    fileDescriptor[1].filename = F("test.txt");
    fileDescriptor[1].url = F("/test.txt");
    fileDescriptor[1].mime = MIME_TEXT_PLAIN;


    fileDescriptor[0].filename = F("logo.jpg");
    fileDescriptor[0].url = F("/logo.jpg");
    fileDescriptor[0].mime = "image/jpg";
    fileDescriptor[0].encode64 = true;

    EMailSender::Attachments attachs = {2, fileDescriptor};

    EMailSender::Response resp = emailSend.send("<receipe@gmail.com>", message, attachs);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
}

void loop()
{

}
