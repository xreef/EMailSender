# EMailSender Library v4.0.0

![EMailSender Logo](https://raw.githubusercontent.com/xreef/EMailSender/master/resources/EMailSenderLogo.jpg)

[![arduino-library-badge](https://www.ardu-badge.com/badge/EMailSender.svg?)](https://www.ardu-badge.com/EMailSender)

A comprehensive Arduino library for sending emails via SMTP with full support for attachments, multiple recipients, and secure connections (SSL/TLS and STARTTLS).

**Author:** Renzo Mischianti  
**Website:** [www.mischianti.org](https://www.mischianti.org)  
**GitHub:** [@xreef](https://github.com/xreef/EMailSender)

---

## 📚 Documentation & Tutorials

Complete tutorials and articles available on [mischianti.org](https://www.mischianti.org):

### General Documentation
- 🌐 **[EMailSender Library Home](https://www.mischianti.org/category/my-libraries/emailsender-send-email-with-attachments/)** - Main library page with all articles
- 📖 **[Library Forum & Support](https://www.mischianti.org/forums/)** - Community support and discussions

### Platform-Specific Guides

#### ESP32 Tutorials
- 📧 **[ESP32: Send email with attachments (Gmail, Yahoo, Outlook)](https://www.mischianti.org/2020/05/16/how-to-send-emails-with-attachments-with-esp32-gmail-and-smtp-server/)** - Complete guide for ESP32
- 🔐 **[ESP32: Secure email with SSL/TLS and attachments](https://www.mischianti.org/2020/05/20/esp32-send-secure-ssl-tls-emails-with-gmail-and-attachments/)** - SSL/TLS configuration
- 📎 **[ESP32: Manage SPIFFS and send attachments](https://www.mischianti.org/2020/05/23/esp32-manage-filesystem-spiffs-and-send-email-attachments/)** - SPIFFS file attachments
- 💾 **[ESP32: LittleFS filesystem and email attachments](https://www.mischianti.org/2021/04/12/esp32-integrated-littlefs-filesystem-email-attachments/)** - LittleFS integration
- 📁 **[ESP32: FFAT filesystem and large file attachments](https://www.mischianti.org/2021/04/19/esp32-integrated-ffat-fat-filesystem-email-attachments/)** - FFAT for large files

#### ESP8266 Tutorials
- 📧 **[ESP8266: Send email with attachments](https://www.mischianti.org/2020/05/30/how-to-send-emails-with-attachments-with-esp8266-gmail-and-smtp/)** - Complete guide for ESP8266
- 📎 **[ESP8266: Manage SPIFFS and send attachments](https://www.mischianti.org/2020/06/03/esp8266-manage-filesystem-spiffs-and-send-email-attachments/)** - SPIFFS integration
- 💾 **[ESP8266: LittleFS filesystem and email attachments](https://www.mischianti.org/2021/04/26/esp8266-integrated-littlefs-filesystem-email-attachments/)** - LittleFS support

#### Arduino Tutorials
- 📧 **[Arduino: Send email with attachments (Ethernet shield)](https://www.mischianti.org/2020/06/09/how-to-send-emails-with-attachments-with-arduino-and-enc28j60/)** - Arduino with ENC28J60
- 🔐 **[Arduino: Secure SSL/TLS email with Ethernet](https://www.mischianti.org/2020/06/13/how-to-send-secure-ssl-tls-emails-with-arduino-and-enc28j60/)** - SSL configuration for Arduino
- 📎 **[Arduino: SD card attachments with Ethernet](https://www.mischianti.org/2020/06/16/arduino-ethernet-enc28j60-send-email-with-sd-attachments/)** - SD card integration
- 📡 **[Arduino: WiFiNINA shield and email sending](https://www.mischianti.org/2020/07/07/how-to-send-emails-with-attachments-with-arduino-mkr-wifi1010-and-wifinina/)** - WiFiNINA shield

#### STM32 Tutorials
- 📧 **[STM32: Send email with W5500 Ethernet](https://www.mischianti.org/2021/02/22/how-to-send-emails-with-attachments-with-stm32-w5500-ethernet/)** - STM32 with W5500
- 🔐 **[STM32: Secure SSL/TLS email with W5500](https://www.mischianti.org/2021/02/25/stm32-send-secure-ssl-tls-emails-with-w5500/)** - SSL/TLS for STM32
- 📎 **[STM32: SPI Flash attachments with email](https://www.mischianti.org/2021/03/01/stm32-spi-flash-filesystem-and-email-attachments/)** - SPI Flash integration
- 🌐 **[STM32: Send email with ENC28J60](https://www.mischianti.org/2021/03/08/how-to-send-emails-with-attachments-with-stm32-enc28j60/)** - ENC28J60 Ethernet

#### Raspberry Pi Pico (RP2040) Tutorials
- 📧 **[Raspberry Pi Pico W: Send email with attachments](https://www.mischianti.org/2022/07/11/raspberry-pi-pico-rp2040-send-email-with-attachments/)** - Pico W WiFi
- 💾 **[Raspberry Pi Pico: LittleFS and email attachments](https://www.mischianti.org/2022/07/18/raspberry-pi-pico-rp2040-littlefs-filesystem-email-attachments/)** - LittleFS integration

### Advanced Topics
- 🔐 **[STARTTLS vs SSL/TLS: Understanding secure email](https://www.mischianti.org/2025/01/04/starttls-vs-ssl-tls-secure-email-arduino/)** - NEW! Security protocols explained
- 📮 **[Gmail App Passwords: Setup guide](https://www.mischianti.org/2020/05/16/gmail-app-passwords-configuration/)** - Configure Gmail for Arduino
- 🛠️ **[Troubleshooting email sending issues](https://www.mischianti.org/2020/06/20/troubleshooting-email-sending-arduino-esp32-esp8266/)** - Common problems and solutions

---

## 📋 Table of Contents
- [Features](#-features)
- [Supported Platforms](#-supported-platforms)
- [Supported Network Interfaces](#-supported-network-interfaces)
- [Supported Storage Systems](#-supported-storage-systems)
- [Installation](#-installation)
- [Basic Usage](#-basic-usage)
- [Changelog v4.0.0](#-changelog-v400)

## ✨ Features

### Core Features
- ✅ **Send emails via SMTP** with authentication
- ✅ **Multiple recipients** (To, Cc, Bcc)
- ✅ **HTML and plain text** email formats
- ✅ **File attachments** from various storage systems
- ✅ **Base64 encoding** for attachments
- ✅ **Custom sender name** and email address
- ✅ **Subject and message body** customization

### Security Features (NEW in v4.0.0)
- ✅ **STARTTLS support** (Port 587) - NEW!
- ✅ **Implicit SSL/TLS** (Port 465)
- ✅ **Multiple authentication methods**:
  - AUTH LOGIN (default)
  - AUTH PLAIN (SASL)
  - CRAM-MD5 (ESP32 only)
- ✅ **Internal SSLClient** for STARTTLS on ESP32/ESP8266
- ✅ **OpenSLab SSLClient** (BearSSL) for Ethernet shields

### Network Support
- Multiple network interfaces (WiFi, Ethernet, WiFiNINA)
- Automatic IP resolution
- Connection retry logic
- Flexible client configuration

## 🎯 Supported Platforms

| Platform | WiFi | Ethernet | SSL/TLS | STARTTLS |
|----------|------|----------|---------|----------|
| **ESP32** | ✅ | ✅ | ✅ | ✅ |
| **ESP8266** | ✅ | ✅ | ✅ | ✅ |
| **Arduino Mega/Uno** | ❌ | ✅ | ❌ | ❌ |
| **Arduino SAMD** | ✅ | ✅ | ✅ | ✅ |
| **STM32** | ❌ | ✅ | ✅ | ✅ |
| **Raspberry Pi Pico (RP2040)** | ✅ | ✅ | ✅ | ✅ |

> **⚠️ IMPORTANTE - Arduino Mega/Uno e SSL/TLS:**
> 
> **Arduino Mega and Uno DO NOT support SSL/TLS** due to severe RAM limitations:
> - Arduino Mega has only **8KB of RAM**
> - Arduino Uno has only **2KB of RAM**
> - BearSSL for SSL/TLS requires at least **24KB of RAM** for encryption buffers
> 
> **Alternative solutions for Arduino Mega/Uno:**
> 1. ✅ **Use non-secure SMTP (port 25)** - Works but not recommended
> 2. ✅ **Use a local SMTP relay** - Configure a local server that handles SSL
> 3. ✅ **Switch to ESP32/ESP8266** - They have more RAM and native SSL/TLS support
> 4. ✅ **Use Arduino SAMD (MKR WiFi 1010)** - Has 32KB RAM and supports SSL/TLS
> 
> **Arduino Mega/Uno examples in this library use only non-secure SMTP!**

## 🌐 Supported Network Interfaces

- **WiFi (ESP32/ESP8266)** - WiFiClient / WiFiClientSecure
- **WiFi (SAMD/MBED)** - WiFiNINA library
- **Ethernet W5100/W5200/W5500** - Standard Ethernet library
- **Ethernet ENC28J60** - UIPEthernet library
- **Ethernet with SSL** - SSLClient wrapper for secure connections

## 💾 Supported Storage Systems

### Internal Storage (Microcontroller Flash)
- **SPIFFS** (ESP32, ESP8266)
- **LittleFS** (ESP32, ESP8266, RP2040)
- **FFAT** (ESP32)
- **SPIFM** (SPI Flash with Adafruit_SPIFlash)

### External Storage (SD/microSD Cards)
- **SD** (Standard SD library)
- **SdFat** (Version 1.x for RP2040/ESP8266)
- **SdFat2** (Version 2.x for modern platforms)

## 📦 Installation

### Arduino IDE
1. Open Arduino IDE
2. Go to **Sketch > Include Library > Manage Libraries**
3. Search for "**EMailSender**"
4. Click **Install**

### PlatformIO
Add to your `platformio.ini`:
```ini
lib_deps = 
    xreef/EMailSender@^4.0.0
```

### Manual Installation
1. Download the library from [GitHub](https://github.com/xreef/EMailSender)
2. Extract to Arduino libraries folder
3. Restart Arduino IDE

## 🚀 Basic Usage

### Simple Email (ESP32 with Gmail)

```cpp
#include <WiFi.h>
#include <EMailSender.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Gmail SMTP configuration
EMailSender emailSend("your.email@gmail.com", 
                      "your_app_password",
                      "your.email@gmail.com", 
                      "Your Name",
                      "smtp.gmail.com", 
                      587);  // Port 587 for STARTTLS

void setup() {
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  
  // Prepare email message
  EMailSender::EMailMessage message;
  message.subject = "Test Email";
  message.message = "Hello from ESP32!";
  
  // Send email
  EMailSender::Response resp = emailSend.send("recipient@example.com", message);
  
  Serial.println("Sending status: ");
  Serial.println(resp.code);
  Serial.println(resp.desc);
  Serial.println(resp.status);
}

void loop() {
  // Nothing here
}
```

### HTML Email with Multiple Recipients

```cpp
EMailSender::EMailMessage message;
message.subject = "HTML Email Test";
message.message = "<h1>Hello!</h1><p>This is an <b>HTML</b> email.</p>";
message.mime = MIME_TEXT_HTML;  // Set MIME type to HTML

// Multiple recipients
const char* recipients[] = {
  "recipient1@example.com",
  "recipient2@example.com",
  "recipient3@example.com"
};

EMailSender::Response resp = emailSend.send(recipients, 3, message);
```

### Email with CC and BCC

```cpp
// Array with To, Cc, and Bcc addresses
const char* allRecipients[] = {
  "to@example.com",      // To (1)
  "cc@example.com",      // Cc (1)
  "bcc@example.com"      // Bcc (1)
};

// send(recipients, numTo, numCc, numBcc, message)
EMailSender::Response resp = emailSend.send(allRecipients, 1, 1, 1, message);
```

### Debug Mode

Enable debugging by uncommenting in `EMailSenderKey.h`:

```cpp
#define EMAIL_SENDER_DEBUG
#define DEBUG_PRINTER Serial
```

This will print detailed SMTP communication to Serial Monitor.

### Memory Issues

For boards with limited RAM (like Arduino Uno):
- Disable attachments if not needed
- Use smaller email messages
- Consider using Arduino Mega or ESP32

### Gmail Specific Issues

**"Username and Password not accepted"**
1. Enable 2-Factor Authentication
2. Generate App Password at https://myaccount.google.com/apppasswords
3. Use the 16-character app password (no spaces)

**"Less secure app access"**
- Google removed "less secure apps" option
- You MUST use App Passwords now
- Regular account password will not work

## 📝 Changelog v4.0.0 - 08/10/2025

### New Features
- ✨ **STARTTLS support (Port 587)** - Full implementation for secure email on port 587
- ✨ **Internal SSLClient integration** - Built-in SSL/TLS support for ESP32/ESP8266
- ✨ **Enhanced authentication** - Better support for various SMTP auth methods
- ✨ **Improved error handling** - More detailed error messages and responses

### Improvements
- 🔧 Unified client handling for better compatibility
- 🔧 Better SSL/TLS handshake management
- 🔧 Improved EHLO/HELO command handling with multi-line response support
- 🔧 Enhanced connection retry logic
- 🔧 Better memory management for large attachments

### Bug Fixes
- 🐛 Fixed SSLClient initialization on ESP32
- 🐛 Fixed STARTTLS upgrade sequence
- 🐛 Corrected client scope issues
- 🐛 Fixed BearSSL integration for Ethernet shields

### Breaking Changes
- ⚠️ Minimum Arduino IDE version: 1.8.x
- ⚠️ ESP32 core minimum version: 2.0.x (for WiFiClientSecure improvements)
- ⚠️ Some internal API changes (user-facing API remains compatible)

### Migration from v3.x
Most code will work without changes. If using custom client configuration:
- Check `EMailSenderKey.h` for new defines
- Update authentication methods if using advanced features
- Test STARTTLS connections (port 587) for better security

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

## 📄 License

MIT License - See LICENSE file for details

Copyright (c) 2017-2025 Renzo Mischianti

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## 📞 Support

- **Website**: https://www.mischianti.org
- **GitHub Issues**: https://github.com/xreef/EMailSender/issues
- **Forum**: https://www.mischianti.org/forums/

## 👤 Author

**Renzo Mischianti**
- Website: https://www.mischianti.org
- Email: renzo.mischianti@gmail.com
- GitHub: [@xreef](https://github.com/xreef)

## ⭐ Show Your Support

If this library helped your project, please ⭐ star it on GitHub!

---

Made with ❤️ by Renzo Mischianti
