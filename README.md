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
