/** \mainpage EMailSender library
 *
 * MIT license
 * written by Renzo Mischianti
 */

#ifndef EMailSender_h
#define EMailSender_h

#include <WiFiClientSecure.h>

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Uncomment if you use esp8266 core <= 2.4.2
// #define ARDUINO_ESP8266_RELEASE_2_4_2

// Uncomment to enable printing out nice debug messages.
// #define EMAIL_SENDER_DEBUG

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
		String subject;
		String message;
	} EMailMessage;

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

	EMailSender::Response send(const char* to, EMailMessage &email);

	void setIsSecure(bool isSecure = false);

private:
	uint16_t smtp_port = 465;
	char* smtp_server = strdup("smtp.gmail.com");
	char* email_login = 0;
	char* email_from  = 0;
	char* email_password = 0;

	bool isSecure = false;

    String _serverResponce;
    Response awaitSMTPResponse(WiFiClientSecure &client, const char* resp = "", const char* respDesc = "", uint16_t timeOut = 10000);
};

#endif
