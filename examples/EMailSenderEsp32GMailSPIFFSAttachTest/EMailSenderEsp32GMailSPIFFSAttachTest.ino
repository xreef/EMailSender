/*
 * EMailSender library for Arduino, esp8266 and esp32
 * esp32 Gmail send example with 2 attach loaded in SPIFFS
 *
 * The base64 encoding of the image is slow, so be patient
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <EMailSender.h>
#include <WiFi.h>

#include <SPIFFS.h>

const char* ssid = "<YOUR-SSID>";
const char* password = "<YOUR-PASSWD>";

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("account_gmail@gmail.com", "<YOUR-GMAIL-PASSWD>");

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

    if(!SPIFFS.begin()){
          Serial.println("An Error has occurred while mounting SPIFFS");
          return;
    }

    Serial.println("ReadDir");
    File dir = SPIFFS.open("/");
    File file = dir.openNextFile();
    while (file) {
        Serial.print(file.name());
        Serial.println(file.size());

        file = dir.openNextFile();
    }

    connection_state = WiFiConnect(ssid, password);
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect

    EMailSender::EMailMessage message;
    message.subject = "Soggetto";
    message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

    EMailSender::FileDescriptior fileDescriptor[2];
    fileDescriptor[1].filename = F("test.txt");
    fileDescriptor[1].url = F("/test.txt");
    fileDescriptor[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_SPIFFS;

    fileDescriptor[0].filename = F("logo.jpg");
    fileDescriptor[0].url = F("/logo.jpg");
    fileDescriptor[0].mime = "image/jpg";
    fileDescriptor[0].encode64 = true;
    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SPIFFS;

    EMailSender::Attachments attachs = {2, fileDescriptor};

    EMailSender::Response resp = emailSend.send("email_to_receive@gmail.com", message, attachs);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
}

void loop()
{

}
