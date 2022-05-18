/**
 * EMailSender ^3.0.0 on STM32
 *
 * Send Email with GMail (application password see the article on how to configure)
 *
 * Need FLASH > 64K if you want use GMail with SSL
 * alternatively you can use SendGrid without activation
 * of SSLClient
 *
 * and ethernet w5500 SS
 *
To activate SSL you must uncomment
#define SSLCLIENT_WRAPPER

#ifndef DEFAULT_EMAIL_NETWORK_TYPE_STM32
	#define DEFAULT_EMAIL_NETWORK_TYPE_STM32 	NETWORK_W5100
	#define DEFAULT_INTERNAL_STM32_STORAGE STORAGE_NONE
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
#include <SPI.h>
#include <EthernetLarge.h>
#include "EMailSender.h"

#define MACADDRESS 0x00,0x01,0x02,0x03,0x04,0x05
#define MYIPADDR 192,168,1,28
#define MYIPMASK 255,255,255,0
#define MYDNS 192,168,1,1
#define MYGW 192,168,1,1

uint8_t macaddress[6] = {MACADDRESS};
EMailSender emailSend("account@gmail.com", "<APPLICATION PASSWORD>");

void setup() {

    Serial.begin(115200);

    while (!Serial) {
	    delay(100);
	  }

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

   EMailSender::EMailMessage message;
   message.subject = "Soggetto";
   message.message = "Ciao come stai<br>io bene.<br>www.mischianti.org";

   // Send to 3 different email
   const char* arrayOfEmail[] = {"destination1@gmail.com", "destination2@yahoo.com", "destination3@other.com"};

   Serial.println("All structure created!");

   EMailSender::Response resp = emailSend.send(arrayOfEmail, 3, message);

   Serial.println("Sending status: ");

   Serial.println(resp.status);
   Serial.println(resp.code);
   Serial.println(resp.desc);

}

void loop() {
  // put your main code here, to run repeatedly:

}
