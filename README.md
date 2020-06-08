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

# Library to send EMail via esp8266. 

### [Updated tutorial on my site](https://www.mischianti.org/2019/09/10/send-email-with-esp8266-and-arduino/)

## Tutorial: 

To download. click the DOWNLOADS button in the top right corner, rename the uncompressed folder EMailSender. Check that the EMailSender folder contains `EMailSender\\.cpp` and `EMailSender.h`. Place the DHT library folder your `<arduinosketchfolder>/libraries/` folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.

# Reef complete EMailSender library to send EMail.
I try to rationalize a famous library like Gsender. 

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
