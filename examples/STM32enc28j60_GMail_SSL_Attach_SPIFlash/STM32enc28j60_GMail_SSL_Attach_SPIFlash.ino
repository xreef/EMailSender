/**
 * EMailSender ^3.0.0 on STM32
 *
 * Send Email with GMail (application password see the article on how to configure)
 * and attach a file from SPI Flash
 *
 * Need FLASH > 64K if you want use GMail with SSL
 * alternatively you can use SendGrid without activation
 * of SSLClient
 *
 * and ethernet enc28j60 SS PA3
 * SPI Flash on standard SPI with SS
 *
 To activate SSL you must uncomment
#define SSLCLIENT_WRAPPER

#ifndef DEFAULT_EMAIL_NETWORK_TYPE_STM32
	#define DEFAULT_EMAIL_NETWORK_TYPE_STM32 	NETWORK_ETHERNET_ENC
	#define DEFAULT_INTERNAL_STM32_STORAGE STORAGE_SPIFM
	#define DEFAULT_EXTERNAL_STM32_STORAGE STORAGE_NONE
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

#include <EthernetEnc.h>

#include "SdFat.h"
#include "Adafruit_SPIFlash.h"

#include <EMailSender.h>

Adafruit_FlashTransport_SPI flashTransport(SS, SPI); // Set CS and SPI interface
Adafruit_SPIFlash flash(&flashTransport);

// file system object from SdFat
FatFileSystem fatfs;

#define ETHERNET_CS_PIN PA3

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

EMailSender emailSend("account@gmail.com", "<APPLICATION PASSWORD>");

void setup() {
	  // Initialize serial port and wait for it to open before continuing.
	  Serial.begin(115200);
	  while (!Serial) {
	    delay(100);
	  }
	  Serial.println("Adafruit SPI Flash FatFs Full Usage Example");

	  // Initialize flash library and check its chip ID.
	  if (!flash.begin()) {
	    Serial.println("Error, failed to initialize flash chip!");
	    while(1) yield();
	  }

	  Serial.print("JEDEC ID: "); Serial.println(flash.getJEDECID(), HEX);
	  Serial.print("Flash size: "); Serial.println(flash.size());
	  Serial.flush();

	  // First call begin to mount the filesystem.  Check that it returns true
	  // to make sure the filesystem was mounted.
	  if (!fatfs.begin(&flash)) {
	    Serial.println("Error, failed to mount newly formatted filesystem!");
	    Serial.println("Was the flash chip formatted with the SdFat_format example?");
	    while(1) yield();
	  }
	  Serial.println("Mounted filesystem!");
	  Serial.println();


    // You can use Ethernet.init(pin) to configure the CS pin
    Ethernet.init(ETHERNET_CS_PIN);

    if (Ethernet.begin(mac)) { // Dynamic IP setup
        Serial.println(F("DHCP OK!"));
    }else{
        Serial.println(F("Failed to configure Ethernet using DHCP"));
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) {
          Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
          while (true) {
            delay(1); // do nothing, no point running without Ethernet hardware
          }
        }
        if (Ethernet.linkStatus() == LinkOFF) {
          Serial.println(F("Ethernet cable is not connected."));
        }

    	  IPAddress ip(MYIPADDR);
    	  IPAddress dns(MYDNS);
    	  IPAddress gw(MYGW);
    	  IPAddress sn(MYIPMASK);
    	  Ethernet.begin(mac, ip, dns, gw, sn);
		  Serial.println("STATIC OK!");
    }
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
    Serial.println();

      // To remove previous test
      // SD.remove(F("/testCreate.txt"));

      File testFile = fatfs.open("/test.txt", FILE_WRITE);

      if (testFile){
          Serial.println("Write file content!");
          testFile.print("Here the test text!! From SPI Flash");

          testFile.close();
      }else{
          Serial.println("Problem on create file!");
      }

      testFile.close();

    delay(5000);


    byte attachsNumber = 1;

    EMailSender::FileDescriptior fileDescriptor[attachsNumber];
    fileDescriptor[0].filename = F("test.txt");
    fileDescriptor[0].url = F("/test.txt");
    fileDescriptor[0].mime = MIME_TEXT_HTML;
    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SPIFM;

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
