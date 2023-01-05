/**
 * EMailSender ^3.0.0 on STM32
 *
 * Send Email with GMail (application password see the article on how to configure)
 * and attach a file from SD
 *
 * Need FLASH > 64K if you want use GMail with SSL
 * alternatively you can use SendGrid without activation
 * of SSLClient
 *
 * and ethernet w5500 SS
 * SD on secondary SPI
 *
To activate SSL you must uncomment
#define SSLCLIENT_WRAPPER

#ifndef DEFAULT_EMAIL_NETWORK_TYPE_STM32
	#define DEFAULT_EMAIL_NETWORK_TYPE_STM32 	NETWORK_W5100
	#define DEFAULT_INTERNAL_STM32_STORAGE STORAGE_NONE
	#define DEFAULT_EXTERNAL_STM32_STORAGE STORAGE_SDFAT2
#endif
 *
 * @author Renzo Mischianti <www.mischianti.org>
 * @details www.mischianti.org
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2022
 *
 */


#include <Arduino.h>
#include <SPI.h>
#include <EthernetLarge.h>
#include <SdFat.h>
#include <sdios.h>

#include "EMailSender.h"
#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

// To use SD with secondary SPI
#define SD_CS_PIN PB12
static SPIClass mySPI2(PB15, PB14, PB13, SD_CS_PIN);
#define SD2_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SD_SCK_MHZ(18), &mySPI2)

SdFat sd;


uint8_t macaddress[6] = {MACADDRESS};
EMailSender emailSend("account@gmail.com", "<APPLICATION PASSWORD>");

void setup() {

    Serial.begin(115200);

    while (!Serial) {
	    delay(100);
	  }
    Serial.println("Begin SD");

  // Secondary SPI for SD
  if (!sd.begin(SD2_CONFIG)) {
  // Primary SPI for SD
  // if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card inserted?"));
    Serial.println(F("* is your wiring correct?"));
    Serial.println(F("* did you change the chipSelect pin to match your shield or module?"));
    while (1);
  } else {
    Serial.println(F("Wiring is correct and a card is present."));
  }

  // Show capacity and free space of SD card
  Serial.print(F("Capacity of card:   ")); Serial.print(long( sd.card()->sectorCount() >> 1 )); Serial.println(F(" kBytes"));

    Serial.println("Begin Ethernet");

    Ethernet.init(PA4);
    if (Ethernet.begin(macaddress)) { // Dynamic IP setup
        Serial.println("DHCP OK!");
    }else{
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
          Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
          while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
          }
        }
        if (Ethernet.linkStatus() == LinkOFF) {
          Serial.println("Ethernet cable is not connected.");
        }

    	  IPAddress ip(MYIPADDR);
    	  IPAddress dns(MYDNS);
    	  IPAddress gw(MYGW);
    	  IPAddress sn(MYIPMASK);
    	  Ethernet.begin(macaddress, ip, dns, gw, sn);
		  Serial.println("STATIC OK!");
    }

      // To remove previous test
      // SD.remove(F("/testCreate.txt"));

      FsFile testFile = sd.open("/test.txt", O_WRITE);

      if (testFile){
          Serial.println("Write file content!");
          testFile.print("Here the test text!! www.mischianti.org From SdFat2");

          testFile.close();
      }else{
          Serial.println("Problem on create file!");
      }

      testFile.close();

    delay(5000);


    Serial.print("Local IP : ");
    Serial.println(Ethernet.localIP());
    Serial.print("Subnet Mask : ");
    Serial.println(Ethernet.subnetMask());
    Serial.print("Gateway IP : ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print("DNS Server : ");
    Serial.println(Ethernet.dnsServerIP());

   Serial.println("Ethernet Successfully Initialized");

    byte attachsNumber = 1;

    EMailSender::FileDescriptior fileDescriptor[attachsNumber];
    fileDescriptor[0].filename = F("test.txt");
    fileDescriptor[0].url = F("/test.txt");
    fileDescriptor[0].mime = MIME_TEXT_HTML;
    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

//    fileDescriptor[1].filename = F("logo.jpg");
//    fileDescriptor[1].url = F("/logo.jpg");
//    fileDescriptor[1].mime = "image/jpg";
//    fileDescriptor[1].encode64 = true;
//     fileDescriptor[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_LITTLE_FS;

    EMailSender::Attachments attachs = {attachsNumber, fileDescriptor};


   EMailSender::EMailMessage message;
   message.subject = "Soggetto";
   message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

   // Send to 3 different email
   const char* arrayOfEmail[] = {"destination1@gmail.com", "destination2@yahoo.com", "destination3@other.com"};

   Serial.println("All structure created!");

   EMailSender::Response resp = emailSend.send(arrayOfEmail, 3, message, attachs);

//    // Send to 3 different email, 2 in C and 1 in CC
//    const char* arrayOfEmail[] = {"mischianti@gmail.com", "smtp.mischianti@gmail.com", "renzo.mischianti@gmail.com"};
//    EMailSender::Response resp = emailSend.send(arrayOfEmail, 2, 1, message);
//
//    // Send to 3 different email first to C second to CC and third to CCn
//    const char* arrayOfEmail[] = {"mischianti@gmail.com", "smtp.mischianti@gmail.com", "renzo.mischianti@gmail.com"};
//    EMailSender::Response resp = emailSend.send(arrayOfEmail, 1, 1, 1, message);


   Serial.println("Sending status: ");

   Serial.println(resp.status);
   Serial.println(resp.code);
   Serial.println(resp.desc);

}

void loop() {
  // put your main code here, to run repeatedly:

}
