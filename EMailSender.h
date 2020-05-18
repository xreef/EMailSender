/** \mainpage EMailSender library
 *
 * MIT license
 * written by Renzo Mischianti
 */

#ifndef EMailSender_h
#define EMailSender_h

#define NETWORK_ESP8266_ASYNC (0)
#define NETWORK_ESP8266 (1)
#define NETWORK_W5100 (2)
#define NETWORK_ENC28J60 (3)
#define NETWORK_ESP32 (4)
#define NETWORK_ESP32_ETH (5)

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#if !defined(EMAIL_NETWORK_TYPE)
// select Network type based
#if defined(ESP8266) || defined(ESP31B)
#define EMAIL_NETWORK_TYPE NETWORK_ESP8266
//#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP8266_ASYNC
//#define WEBSOCKETS_NETWORK_TYPE NETWORK_W5100

#elif defined(ESP32)
#define EMAIL_NETWORK_TYPE NETWORK_ESP32
//#define WEBSOCKETS_NETWORK_TYPE NETWORK_ESP32_ETH
#else
#define EMAIL_NETWORK_TYPE NETWORK_ENC28J60

#endif
#endif

// Includes and defined based on Network Type
#if(EMAIL_NETWORK_TYPE == NETWORK_ESP8266_ASYNC)

// Note:
//   No SSL/WSS support for client in Async mode
//   TLS lib need a sync interface!

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>

#define EMAIL_NETWORK_CLASS WiFiClient
#define EMAIL_NETWORK_SSL_CLASS WiFiClientSecure
#define EMAIL_NETWORK_SERVER_CLASS WiFiServer

#elif defined(ESP31B)
#include <ESP31BWiFi.h>
#else
#error "network type ESP8266 ASYNC only possible on the ESP mcu!"
#endif
//
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncTCPbuffer.h>
//#define EMAIL_NETWORK_CLASS AsyncTCPbuffer
//#define EMAIL_NETWORK_SERVER_CLASS AsyncServer

#elif(EMAIL_NETWORK_TYPE == NETWORK_ESP8266)

#if !defined(ESP8266) && !defined(ESP31B)
#error "network type ESP8266 only possible on the ESP mcu!"
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <ESP31BWiFi.h>
#endif
#define EMAIL_NETWORK_CLASS WiFiClient
#define EMAIL_NETWORK_SSL_CLASS WiFiClientSecure
#define EMAIL_NETWORK_SERVER_CLASS WiFiServer

#elif(EMAIL_NETWORK_TYPE == NETWORK_W5100)

#ifdef STM32_DEVICE
#define EMAIL_NETWORK_CLASS TCPClient
#define EMAIL_NETWORK_SERVER_CLASS TCPServer
#else
#include <Ethernet.h>
#include <SPI.h>
#define EMAIL_NETWORK_CLASS EthernetClient
#define EMAIL_NETWORK_SERVER_CLASS EthernetServer
#endif

#elif(EMAIL_NETWORK_TYPE == NETWORK_ENC28J60)

#include <UIPEthernet.h>

#define EMAIL_NETWORK_CLASS UIPClient
#define EMAIL_NETWORK_SERVER_CLASS UIPServer

//#include <UIPEthernet.h>
//UIPClient base_client;
//SSLClient client(base_client, TAs, (size_t)TAs_NUM, A5);
//
//#define EMAIL_NETWORK_CLASS SSLClient
//#define EMAIL_NETWORK_SERVER_CLASS UIPServer

#elif(EMAIL_NETWORK_TYPE == NETWORK_ESP32)

#include <WiFi.h>
#include <WiFiClientSecure.h>
#define EMAIL_NETWORK_CLASS WiFiClient
#define EMAIL_NETWORK_SSL_CLASS WiFiClientSecure
#define EMAIL_NETWORK_SERVER_CLASS WiFiServer

#elif(EMAIL_NETWORK_TYPE == NETWORK_ESP32_ETH)

#include <ETH.h>
#define EMAIL_NETWORK_CLASS WiFiClient
#define EMAIL_NETWORK_SERVER_CLASS WiFiServer

#else
#error "no network type selected!"
#endif

#ifdef EMAIL_NETWORK_SSL_CLASS
#define EMAIL_NETWORK_CLASS EMAIL_NETWORK_SSL_CLASS
#endif

// Uncomment if you use esp8266 core <= 2.4.2
 #define ARDUINO_ESP8266_RELEASE_2_4_2

// Uncomment to enable printing out nice debug messages.
 #define EMAIL_SENDER_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef EMAIL_SENDER_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

class EMailSender {
public:
	EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* smtp_server, uint16_t smtp_port, bool isSecure = false);
	EMailSender(const char* email_login, const char* email_password, const char* email_from, bool isSecure = false);
	EMailSender(const char* email_login, const char* email_password, bool isSecure = false);

	typedef struct {
		String mime = "text/html";
		String subject;
		String message;
	} EMailMessage;

//	typedef struct {
//		String
//	} Attach;

	typedef struct {
		String code;
		String desc;
		bool status = false;
	} Response;

	void setSMTPPort(uint16_t smtp_port);
	void setSMTPServer(const char* smtp_server);
	void setEMailLogin(const char* email_login);
	void setEMailFrom(const char* email_from);
	void setEMailPassword(const char* email_password);

	EMailSender::Response send(const char* to, EMailMessage &email,  const char* publicIP = "mischianti");

	void setIsSecure(bool isSecure = false);

	void setUseAuth(bool useAuth = true) {
		this->useAuth = useAuth;
	}

private:
	uint16_t smtp_port = 465;
	char* smtp_server = strdup("smtp.gmail.com");
	char* email_login = 0;
	char* email_from  = 0;
	char* email_password = 0;

	bool isSecure = false;

	bool useAuth = true;

    String _serverResponce;

    Response awaitSMTPResponse(EMAIL_NETWORK_CLASS &client, const char* resp = "", const char* respDesc = "", uint16_t timeOut = 10000);
};

#endif
