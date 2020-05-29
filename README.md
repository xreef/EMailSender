<div>
<a href="https://www.mischianti.org/forum/forum/xreef-emailsender/"><img
  src="https://github.com/xreef/LoRa_E32_Series_Library/raw/master/resources/mischaintiForumSupport.png" alt="Support forum e32 English"
   align="right"></a>
</div>
<div>
<a href="https://www.mischianti.org/forum/forum/xreef-emailsender-2/"><img
  src="https://github.com/xreef/LoRa_E32_Series_Library/raw/master/resources/buttonSupportoForum.png" alt="Forum supporto e32 italiano"
  align="right"></a>
</div>

# Library to send EMail with attachments via Arduino (support W5100 like, and ENC28J60 via UIPEthernet), esp8266 (SPIFFS and SD) (core <=2.4.2 must be set and >2.4.2) and esp32 (SPIFFS and SD). 

### [Updated tutorial on my site](https://www.mischianti.org/2019/09/10/send-email-with-esp8266-and-arduino/)

## Tutorial: 

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder EMailSender. Check that the EMailSender folder contains `EMailSender\\.cpp` and `EMailSender.h`. Place the DHT library folder your `<arduinosketchfolder>/libraries/` folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.

# Reef complete EMailSender library to send EMail.
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
	- SD (must be selected for 2.4.2 upper version)
	- SPIFFS (default for core 2.4.2 upper version)

esp32
Storage supported
	- SD
	- SPIFFS

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

Send message:
```cpp
    EMailSender::Response resp = emailSend.send("account_to_send@gmail.com", message);
```

Then check the response:
```cpp
    Serial.println("Sending status: ");
    Serial.println(resp.code);
    Serial.println(resp.desc);
    Serial.println(resp.status);
```

Example output:

```cpp
Connection: ESTABLISHED
Got IP address: 192.168.1.104
Sending status: 
1
0
Message sent!
```
