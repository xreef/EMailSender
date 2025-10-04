# EMailSender Library v4.0.0

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
- [SSL/TLS Configuration](#-ssltls-configuration)
- [Advanced Features](#-advanced-features)
- [Examples](#-examples)
- [API Reference](#-api-reference)
- [Troubleshooting](#-troubleshooting)
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
| **Arduino Mega/Uno** | ❌ | ✅ | ✅* | ✅* |
| **Arduino SAMD** | ✅ | ✅ | ✅ | ✅ |
| **STM32** | ❌ | ✅ | ✅ | ✅ |
| **Raspberry Pi Pico (RP2040)** | ✅ | ✅ | ✅ | ✅ |

\* With SSLClient library for Ethernet shields

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

## 🔐 SSL/TLS Configuration

### Port 587 (STARTTLS) - Recommended for Gmail

STARTTLS starts with a plain connection and upgrades to encrypted.

```cpp
// Enable internal SSLClient in EMailSenderKey.h
#define EMAIL_ENABLE_INTERNAL_SSLCLIENT

// Use port 587
EMailSender emailSend("user@gmail.com", "app_password", 
                      "user@gmail.com", "Sender Name",
                      "smtp.gmail.com", 587);

// Enable EHLO (required for STARTTLS)
emailSend.setEHLOCommand(true);
```

### Port 465 (Implicit SSL/TLS)

Connection is encrypted from the start.

```cpp
// Use WiFiClientSecure (ESP32/ESP8266)
EMailSender emailSend("user@gmail.com", "app_password",
                      "user@gmail.com", "Sender Name", 
                      "smtp.gmail.com", 465);

// No additional configuration needed
```

### Using SSLClient with Ethernet (Arduino/STM32)

For boards without native SSL support:

```cpp
#define EMAIL_ENABLE_OPENSLAB_SSLCLIENT

// The library will use BearSSL-based SSLClient
// for secure connections over Ethernet
```

### Gmail Configuration

To use Gmail, you need an **App Password**:

1. Enable 2-Factor Authentication on your Google account
2. Go to [Google App Passwords](https://myaccount.google.com/apppasswords)
3. Generate a new app password
4. Use this 16-character password in your code

**Gmail Settings:**
- SMTP Server: `smtp.gmail.com`
- Port: `587` (STARTTLS) or `465` (SSL/TLS)
- Authentication: Required

## 🎨 Advanced Features

### Custom Authentication Methods

```cpp
// Use AUTH PLAIN (SASL)
emailSend.setSASLLogin(true);

// Use CRAM-MD5 (ESP32 only)
emailSend.setCramMD5Login(true);

// Disable authentication (open relay)
emailSend.setUseAuth(false);
```

### EHLO Configuration

```cpp
// Use EHLO instead of HELO (recommended for modern servers)
emailSend.setEHLOCommand(true);

// Set custom identifier for HELO/EHLO
emailSend.setPublicIpDescriptor("mydevice");
```

### Email with Attachments from SD Card

```cpp
#include <SD.h>

// Initialize SD card
SD.begin(SD_CS_PIN);

// Prepare attachment
EMailSender::FileDescriptior fileDescriptor;
fileDescriptor.filename = "photo.jpg";
fileDescriptor.url = "/photo.jpg";  // Path on SD card
fileDescriptor.mime = MIME_IMAGE_JPG;
fileDescriptor.encode64 = true;  // Base64 encode
fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

EMailSender::Attachments attachments;
attachments.number = 1;
attachments.fileDescriptor = &fileDescriptor;

// Send email with attachment
EMailSender::Response resp = emailSend.send("recipient@example.com", 
                                            message, 
                                            attachments);
```

### Multiple Attachments

```cpp
EMailSender::FileDescriptior files[3];

files[0].filename = "document.pdf";
files[0].url = "/docs/document.pdf";
files[0].mime = "application/pdf";
files[0].encode64 = true;
files[0].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

files[1].filename = "image.png";
files[1].url = "/images/image.png";
files[1].mime = MIME_IMAGE_PNG;
files[1].encode64 = true;
files[1].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

files[2].filename = "data.txt";
files[2].url = "/data.txt";
files[2].mime = MIME_TEXT_PLAIN;
files[2].encode64 = false;
files[2].storageType = EMailSender::EMAIL_STORAGE_TYPE_SD;

EMailSender::Attachments attachments;
attachments.number = 3;
attachments.fileDescriptor = files;

EMailSender::Response resp = emailSend.send("recipient@example.com", 
                                            message, 
                                            attachments);
```

### Attachments from SPIFFS/LittleFS (ESP32/ESP8266)

```cpp
#include <SPIFFS.h>

// Initialize SPIFFS
SPIFFS.begin();

EMailSender::FileDescriptior fileDescriptor;
fileDescriptor.filename = "config.json";
fileDescriptor.url = "/config.json";
fileDescriptor.mime = "application/json";
fileDescriptor.encode64 = false;
fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_SPIFFS;

// Or use LittleFS
// fileDescriptor.storageType = EMailSender::EMAIL_STORAGE_TYPE_LITTLE_FS;
```

## 📚 Examples

The library includes examples for various platforms:

### ESP32 Examples
- `Esp32GMailTest.ino` - Basic Gmail test
- `Esp32GMailSPIFFSAttachTest.ino` - Attachments from SPIFFS
- `Esp32GMailLittleFSAttach.ino` - Attachments from LittleFS
- `Esp32GMailFFatAttachTest.ino` - Attachments from FFAT
- `ESP32_STARTTLS_587.ino` - STARTTLS on port 587 (NEW!)

### ESP8266 Examples
- `Esp8266GMailTest.ino` - Basic Gmail test
- `Esp8266GMailSPIFFSAttachTest.ino` - SPIFFS attachments
- `Esp8266GMailLittleFSAttachTest.ino` - LittleFS attachments
- `ESP8266_TLS_465.ino` - TLS on port 465

### Arduino Examples
- `ArduinoMegaUIPSDAttachTest.ino` - Ethernet + SD attachments
- `ArduinoMega_W5500_TLS_465_OSU.ino` - W5500 with SSL

### SAMD Examples
- `ArduinoSAMDWiFiNINAAttachSD.ino` - WiFiNINA + SD
- `SAMD_WiFiNINA_TLS_465.ino` - TLS connection

### STM32 Examples
- `STM32w5500_GMail_SSL.ino` - W5500 with SSL
- `STM32enc28j60_GMail_SSL_Attach_SPIFlash.ino` - ENC28J60 + SPI Flash

### Raspberry Pi Pico Examples
- `Raspberry_Pi_Pico_GMail_LittleFS.ino` - LittleFS attachments

## 📖 API Reference

### Constructor

```cpp
EMailSender(const char* email_login, 
            const char* email_password);

EMailSender(const char* email_login, 
            const char* email_password,
            const char* email_from);

EMailSender(const char* email_login, 
            const char* email_password,
            const char* email_from,
            const char* name_from);

EMailSender(const char* email_login, 
            const char* email_password,
            const char* email_from,
            const char* name_from,
            const char* smtp_server,
            uint16_t smtp_port);
```

### Configuration Methods

```cpp
void setSMTPServer(const char* smtp_server);
void setSMTPPort(uint16_t smtp_port);
void setEMailLogin(const char* email_login);
void setEMailFrom(const char* email_from);
void setNameFrom(const char* name_from);
void setEMailPassword(const char* email_password);

void setUseAuth(bool useAuth);
void setEHLOCommand(bool useEHLO);
void setSASLLogin(bool isSASLLogin);
void setCramMD5Login(bool onoff);  // ESP32 only

void setPublicIpDescriptor(const char* descriptor);
void setAdditionalResponseLineOnConnection(uint8_t numLines);
void setAdditionalResponseLineOnHELO(uint8_t numLines);
```

### Send Methods

```cpp
// Single recipient
Response send(const char* to, EMailMessage &email, Attachments att = {0, 0});
Response send(String to, EMailMessage &email, Attachments att = {0, 0});

// Multiple recipients (To only)
Response send(const char* to[], byte sizeOfTo, EMailMessage &email, Attachments att = {0, 0});
Response send(String to[], byte sizeOfTo, EMailMessage &email, Attachments att = {0, 0});

// Multiple recipients with Cc
Response send(const char* to[], byte sizeOfTo, byte sizeOfCc, EMailMessage &email, Attachments att = {0, 0});

// Multiple recipients with Cc and Bcc
Response send(const char* to[], byte sizeOfTo, byte sizeOfCc, byte sizeOfCCn, EMailMessage &email, Attachments att = {0, 0});
```

### Data Structures

```cpp
// Email Message
struct EMailMessage {
    String mime = "text/html";  // or "text/plain"
    String subject;
    String message;
};

// File Descriptor for Attachments
struct FileDescriptior {
    StorageType storageType = EMAIL_STORAGE_TYPE_SD;
    String mime;
    bool encode64 = false;
    String filename;
    String url;  // Path to file
};

// Attachments
struct Attachments {
    byte number;
    FileDescriptior *fileDescriptor;
};

// Response
struct Response {
    String code;
    String desc;
    bool status = false;
};
```

### Storage Types

```cpp
enum StorageType {
    EMAIL_STORAGE_TYPE_SPIFFS,
    EMAIL_STORAGE_TYPE_LITTLE_FS,
    EMAIL_STORAGE_TYPE_FFAT,
    EMAIL_STORAGE_TYPE_SPIFM,
    EMAIL_STORAGE_TYPE_SD
};
```

### MIME Types

Predefined MIME type constants:
```cpp
MIME_TEXT_HTML    // "text/html"
MIME_TEXT_PLAIN   // "text/plain"
MIME_IMAGE_JPG    // "image/jpg"
MIME_IMAGE_PNG    // "image/png"
```

## 🔧 Configuration File (EMailSenderKey.h)

The library uses a configuration file to enable/disable features:

```cpp
// Enable STARTTLS support with internal SSLClient (ESP32/ESP8266)
#define EMAIL_ENABLE_INTERNAL_SSLCLIENT

// Enable OpenSLab SSLClient for Ethernet shields (Arduino/STM32)
#define EMAIL_ENABLE_OPENSLAB_SSLCLIENT

// Enable attachments support
#define ENABLE_ATTACHMENTS

// Enable date header management
#define MANAGE_DATE_HEADER

// Network type (auto-detected based on platform)
// EMAIL_NETWORK_TYPE can be:
// - NETWORK_ESP8266 (ESP8266 WiFi)
// - NETWORK_ESP32 (ESP32 WiFi)
// - NETWORK_W5100 (Ethernet W5100/W5200/W5500)
// - NETWORK_ENC28J60 (ENC28J60 Ethernet)
// - NETWORK_WiFiNINA (SAMD/Arduino WiFiNINA)
// - etc.

// Storage types
// INTERNAL_STORAGE can be: STORAGE_SPIFFS, STORAGE_LITTLEFS, STORAGE_FFAT
// EXTERNAL_STORAGE can be: STORAGE_SD, STORAGE_SDFAT2
```

## 🐛 Troubleshooting

### Common Issues

**1. "Could not connect to mail server"**
- Check WiFi/Ethernet connection
- Verify SMTP server address and port
- Check firewall settings
- Try resolving IP manually

**2. "SMTP AUTH error"**
- Verify username and password
- For Gmail, use App Password (not account password)
- Check authentication method (try different auth types)

**3. "STARTTLS command failed"**
- Ensure `EMAIL_ENABLE_INTERNAL_SSLCLIENT` is defined
- Use `setEHLOCommand(true)` for STARTTLS
- Verify port 587 is used (not 465)

**4. "SSL connection failed"**
- For ESP32/ESP8266: Increase SSL buffer size
- Try `setInsecure()` for testing (not for production)
- Check server SSL certificate validity

**5. Compilation errors**
- Make sure all dependencies are installed (SD, SPI, WiFi libraries)
- Check that the correct board is selected
- Update ESP32/ESP8266 core to latest version

**6. Attachment errors**
- Verify file exists on storage
- Check file path (use absolute paths: "/filename.ext")
- Ensure storage is properly initialized before sending
- Check available memory (large attachments need more RAM)

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

## 📝 Changelog v4.0.0

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
