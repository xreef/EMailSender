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
	EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* smtp_server, uint16_t smtp_port);
	EMailSender(const char* email_login, const char* email_password, const char* email_from);
	EMailSender(const char* email_login, const char* email_password);

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
private:
	uint16_t smtp_port = 465;
	char* smtp_server = strdup("smtp.gmail.com");
	char* email_login;
	char* email_from;
	char* email_password;

    String _serverResponce;
    Response awaitSMTPResponse(WiFiClientSecure &client, const char* resp = "", const char* respDesc = "", uint16_t timeOut = 10000);
};

#endif
