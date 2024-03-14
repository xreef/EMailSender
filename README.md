<div>
<a href="https://www.mischianti.org/forums/forum/mischiantis-libraries/emailsender-send-email-with-attachments/"><img
  src="https://github.com/xreef/LoRa_E32_Series_Library/raw/master/resources/buttonSupportForumEnglish.png" alt="Support forum EMailSender English"
   align="right"></a>
</div>
<div>
<a href="https://www.mischianti.org/it/forums/forum/le-librerie-di-mischianti/emailsender-invio-di-email-con-allegati/"><img
  src="https://github.com/xreef/LoRa_E32_Series_Library/raw/master/resources/buttonSupportForumItaliano.png" alt="Forum supporto EMailSender italiano"
  align="right"></a>
</div>



#
#

# Library to send EMail with attachments 
Arduino (support W5100 like must be set, and ENC28J60 via UIPEthernet), esp8266 (SPIFFS, LittleFS and SD) (core <=2.4.2 must be set) and esp32 (SPIFFS, LITTLEFS, Ffat and SD), Arduino WiFiNINA SAMD devices (Arduino MKR WiFi 1010, Vidor 4000 UNO WiFi Rev.2 ecc.) (SD). 

### Complete english tutorial
#### [Send email with attachments (EMailSender v2.x library): Arduino Ethernet](https://www.mischianti.org/2020/06/09/send-email-with-attachments-v2-x-library-arduino-ethernet-part-1/)
#### [Send email with attachments (EMailSender v2.x library): esp32 and esp8266](https://www.mischianti.org/2020/06/16/send-email-with-attachments-emailsender-v2-x-library-esp32-and-esp8266-part-2/)

### Tutorial completo in italiano
#### [Inviare email con allegati (libreria v2.x): Arduino Ethernet](https://www.mischianti.org/it/2020/06/09/inviare-email-con-allegati-libreria-v2-x-arduino-ethernet-part-1/)
#### [Inviare email con allegati (libreria v2.x): esp32 e esp8266](https://www.mischianti.org/it/2020/06/16/inviare-email-con-allegati-libreria-v2-x-esp32-e-esp8266-part-2/)

## Change log
 - 14/03/2024: v3.0.14 Support Arduino GIGA WiFi
 - 31/10/2023: v3.0.13 Fix wrong implementation of FORCE_DISABLE_SSL
 - 17/10/2023: v3.0.12 Fix warnigs of variable not used #43 thanks to @Patriboom @Andy2015
 - 16/02/2023: v3.0.11 Add support for Ethernet_Generic
 - 20/01/2023: v3.0.10 Add the management of ESP8266SdFat library on Raspberry Pi Pico [#forum](https://www.mischianti.org/forums/topic/raspberry-pico-w-emailsender)
 - 17/01/2023: v3.0.10 Add the mamagement of auto check of response number #36 thanks to @HACKER-3000 @H3
 - 13/01/2023: v3.0.9 Fix external storage error 404 ([#forum](https://www.mischianti.org/forums/topic/email-with-sd-files/)
 - 05/01/2023: v3.0.8 Fix distribution list examples
 - 22/09/2022: v3.0.7 Add Raspberry Pi Pico W and rp2040 boards
 - 20/09/2022: v3.0.7 Add STM32 management for https://github.com/rogerclarkmelbourne/Arduino_STM32 library.
 - 16/08/2022: v3.0.6 Add FORCE_DISABLE_SSL to disable SSL if present
 - 19/07/2022: v3.0.5 Additional fix on additional response management ([#26](https://github.com/xreef/EMailSender/issues/26))
 - 12/07/2022: v3.0.4 Fixed attachment issue on SPIFFS and LittleFS
 - 06/07/2022: v3.0.3 Manage multiple response message after connect and HELO ([#26](https://github.com/xreef/EMailSender/issues/26))
 - 03/06/2022: v3.0.2 Add possibility to set additionalResponseLineOnConnection with void setAdditionalResponseLineOnConnection(uint8_t numLines = 0), needed if on connection you receive 220 response as error, and add Ethernet2.
 - 20/05/2022: v3.0.1 Put inside scope the client declaration (if you don't use SSLClient) for backward compatibility
 - 18/05/2022: v3.0.0 Add STM32 support. Add SSLClient integration to allow send Email with SSL encryption (like Gmail) with ethernet (tested with STM32, ESP32, w5500, and enc28j60), minor fix
 - 12/04/2021: v2.4.3 Fix multiple email send
 - 12/04/2021: v2.4.1 Add support for LITTLEFS and Ffat on esp32 and fix certificate verification from esp32 core 1.0.5 
 - 18/03/2021: v2.3.0 Add support for LittleFS on esp8266
 - 02/01/2021: v2.2.0 New support for SAMD devices via WiFiNINA (Arduino MKR WiFi 1010, Arduino Vidor 4000 and Arduino UNO WiFi Rev.2 ecc.).
 - 01/01/2021: v2.1.5 Add possibility to use EHLO instead of HELO, and SASL login.
 - 27/11/2020: v2.1.4 Add more constructor to allow more structure for distribution list (now const char*, char* and String array are supported)

23/11/2020: v2.1.3 Add management of name on EMail from 

## Installation Tutorial: 

To download. 
click the DOWNLOADS button in the top right corner, rename the uncompressed folder EMailSender. 
Check that the EMailSender folder contains `EMailSender\\.cpp` and `EMailSender.h`. 
Place the EMailSender library folder your `<arduinosketchfolder>/libraries/` folder. 
You may need to create the libraries subfolder if its your first library. 
Restart the IDE.

# EMailSender library to send EMail.
With this library you can send email with attach:
Arduino
Network supported
	- w5100 like shield with Ethernet library
	- enc28J60 with UIPLibrary
Storage support
	- SD
 
esp8266
you must pay attention, older core from 2.4.2 must be activated
Storage supported
	- SD 
	- SPIFFS
	- LittleFS 

esp32
Storage supported
	- SD
	- SPIFFS
	- LITTLEFS
	- Ffat

Constructor:
Default value is quite simple and use GMail as smtp server. 
```cpp
	EMailSender emailSend("smtp.account@gmail.com", "password");
```

If you want use onother provider you can use more complex (but simple) contructor
```cpp
	EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* smtp_server, uint16_t smtp_port);

```

You must connect to WIFI :P.

Create a message with the structure EMailMessage
```cpp
    EMailSender::EMailMessage message;
    message.subject = "Subject";
    message.message = "Hi, How are you<br>Fine.";
```

Create array of attachments
```cpp
// 		Two file
    EMailSender::FileDescriptior fileDescriptor[2];
    fileDescriptor[1].filename = F("test.txt");
    fileDescriptor[1].url = F("/test.txt");
    fileDescriptor[1].mime = MIME_TEXT_PLAIN;
    fileDescriptor[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

    fileDescriptor[0].filename = F("logo.jpg");
    fileDescriptor[0].url = F("/logo.jpg");
    fileDescriptor[0].mime = "image/jpg";
    fileDescriptor[0].encode64 = true;
    fileDescriptor[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

    EMailSender::Attachments attachs = {2, fileDescriptor};
```

Send message:
```cpp
    EMailSender::Response resp = emailSend.send("account_to_send@gmail.com", message, attachs);
```

Then check the response:
```cpp
    Serial.println("Sending status: ");
    Serial.println(resp.code);
    Serial.println(resp.desc);
    Serial.println(resp.status);
```

From version 2.1.1 new features distribution list to send CC and CCn email.

Example output:

```cpp
Connection: ESTABLISHED
Got IP address: 192.168.1.104
Sending status: 
1
0
Message sent!
```

https://downloads.arduino.cc/libraries/logs/github.com/xreef/EMailSender/