/*
 * EMail Sender Arduino, esp8266 and esp32 library to send email
 *
 * AUTHOR:  Renzo Mischianti
 * VERSION: 2.4.3
 *
 * https://www.mischianti.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Renzo Mischianti www.mischianti.org All right reserved.
 *
 * You may copy, alter and reuse this code in any way you like, but please leave
 * reference to www.mischianti.org in your comments if you redistribute this code.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef EMailSenderKey_h
#define EMailSenderKey_h

// Uncomment if you use esp8266 core <= 2.4.2
//#define ARDUINO_ESP8266_RELEASE_2_4_2

#define ENABLE_ATTACHMENTS

// Uncomment to enable printing out nice debug messages.
//#define EMAIL_SENDER_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

#define STORAGE_SPIFFS (0)
#define STORAGE_LITTLEFS (1)
#define STORAGE_FFAT (2)

#define NETWORK_ESP8266_ASYNC (0)
#define NETWORK_ESP8266 (1)
#define NETWORK_ESP8266_242 (6)
#define NETWORK_W5100 (2)
#define NETWORK_ENC28J60 (3)
#define NETWORK_ESP32 (4)
#define NETWORK_ESP32_ETH (5)
#define NETWORK_WiFiNINA (7)
#define NETWORK_ETHERNET_LARGE (8)

#define SSLCLIENT_WRAPPER

#ifndef DEFAULT_EMAIL_NETWORK_TYPE_ESP8266
	#define DEFAULT_EMAIL_NETWORK_TYPE_ESP8266 	NETWORK_ESP8266
	#define DEFAULT_INTERNAL_ESP8266_STORAGE STORAGE_SPIFFS
#endif
#ifndef DEFAULT_EMAIL_NETWORK_TYPE_ESP32
	#define DEFAULT_EMAIL_NETWORK_TYPE_ESP32 	NETWORK_ESP32
	#define DEFAULT_INTERNAL_ESP32_STORAGE STORAGE_SPIFFS
#endif
#ifndef DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO
	#define DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO 	NETWORK_W5100
#endif
#ifndef DEFAULT_EMAIL_NETWORK_TYPE_ARDUINO_SAMD
	#define DEFAULT_EMAIL_NETWORK_TYPE_SAMD 	NETWORK_ETHERNET_LARGE
#endif

#define SD_CS_PIN 4

//#define STORAGE_INTERNAL_FORCE_DISABLE
//#define STORAGE_SD_FORCE_DISABLE

#endif
