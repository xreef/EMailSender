/*
 * EMailSender library for Arduino, esp8266 and esp32
 * Arduino Mega and UIPEthernet send example with attach
 * this example is not tested for all, I can't find a provider
 * that manage attach without SSL and TLS
 *
 * Pay attention you must set in the library
 * #define DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO 	NETWORK_ENC28J60
 * for UIPEthernet
 *
 * #define DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO 	NETWORK_W5100
 * for standard Ethernet
 *
 * The base64 encoding of the image is slow, so be patient
 *
 *
 * https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/
 *
 */

#include "Arduino.h"
#include <SPI.h>
#include <UIPEthernet.h>
#include <SD.h>

#include <EMailSender.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//	EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* smtp_server, uint16_t smtp_port, bool isSecure = false);
EMailSender emailSend("<LOGIN>", "<PASSWD>", "<EMAIL-FROM>", "<SMTP-SERVER>", "<SMTP-SERVER-PORT>");

void printDirectory(File dir, int numTabs);

//The setup function is called once at startup of the sketch
void setup()
{
    Serial.begin(115200);

    delay(2000);

    Serial.println("Starting!");

    Serial.print("Initializing SD card...");

    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");

    File root = SD.open("/");

    printDirectory(root, 0);

    Serial.println("done!");

//	File myFile = SD.open("/TEST.TXT", "r");
//	  if(myFile) {
//		  myFile.seek(0);
//		  DEBUG_PRINTLN(F("OK"));
//		myFile.close();
//	  }
//	  else {
//		  DEBUG_PRINTLN(F("KO"));
//	  }
//
//
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
    message.mime = MIME_TEXT_PLAIN;

// 		Two file
//    EMailSender::FileDescriptior fileDescriptor[2];
//    fileDescriptor[1].filename = F("test.txt");
//    fileDescriptor[1].url = F("/test.txt");
//    fileDescriptor[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;
//
//    fileDescriptor[0].filename = F("logo.jpg");
//    fileDescriptor[0].url = F("/logo.jpg");
//    fileDescriptor[0].mime = "image/jpg";
//    fileDescriptor[0].encode64 = true;
//    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;
//
//    EMailSender::Attachments attachs = {2, fileDescriptor};

// 		One file
    EMailSender::FileDescriptior fileDescriptor[1];
    fileDescriptor[0].filename = F("test.txt");
    fileDescriptor[0].url = F("/test.txt");
    fileDescriptor[0].mime = MIME_TEXT_PLAIN;
    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

    // Pay attention base64 encoding is quite slow
//	EMailSender::FileDescriptior fileDescriptor[2];
//	fileDescriptor[0].filename = F("logo.jpg");
//	fileDescriptor[0].url = F("/logo.jpg");
//	fileDescriptor[0].mime = "image/jpg";
//	fileDescriptor[0].encode64 = false;
//	fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

    EMailSender::Attachments attachs = {1, fileDescriptor};

    EMailSender::Response resp = emailSend.send("email_to_receive@gmail.com", message, attachs);

    Serial.println("Sending status: ");

    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);

    File root2 = SD.open("/");

    printDirectory(root2, 0);

    Serial.println("done!");
    SD.end();
}

void loop()
{

}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


