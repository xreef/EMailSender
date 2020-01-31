#include "EMailSender.h"

// BASE64 -----------------------------------------------------------
const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                    "abcdefghijklmnopqrstuvwxyz"
                                    "0123456789+/";

#define encode64(arr) encode64_f(arr,strlen(arr))

inline void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

int base64_encode(char *output, char *input, int inputLen) {
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while (inputLen--) {
    a3[i++] = *(input++);
    if (i == 3) {
      a3_to_a4(a4, a3);

      for (i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for (j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while ((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int base64_enc_length(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

const char* encode64_f(char* input, uint8_t len) {
  // encoding

	DEBUG_PRINTLN(F("Encoding"));

	DEBUG_PRINTLN(input);
	DEBUG_PRINTLN(len);

  //int encodedLen =
 base64_enc_length(len);
  static char encoded[256];
  // note input is consumed in this step: it will be empty afterwards
  base64_encode(encoded, input, len);
  return encoded;
}

// END BASE64 ---------------------------------------------------------

EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from, // @suppress("Class members should be properly initialized")
		const char* smtp_server, uint16_t smtp_port, bool isSecure) {
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);
	this->setSMTPServer(smtp_server);
	this->setSMTPPort(smtp_port);

	this->isSecure = isSecure;
}

EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from, bool isSecure) { // @suppress("Class members should be properly initialized")
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);

	this->isSecure = isSecure;
}

EMailSender::EMailSender(const char* email_login, const char* email_password, bool isSecure){ // @suppress("Class members should be properly initialized")
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_login);
	this->setEMailPassword(email_password);

	this->isSecure = isSecure;
}

void EMailSender::setSMTPPort(uint16_t smtp_port){
	this->smtp_port = smtp_port;
};
void EMailSender::setSMTPServer(const char* smtp_server){
	delete [] this->smtp_server;
	this->smtp_server = new char[strlen(smtp_server)+1];
	strcpy(this->smtp_server, smtp_server);
};

void EMailSender::setEMailLogin(const char* email_login){
	delete [] this->email_login;
	this->email_login = new char[strlen(email_login)+1];
	strcpy(this->email_login, email_login);
};
void EMailSender::setEMailFrom(const char* email_from){
	delete [] this->email_from;
	this->email_from = new char[strlen(email_from)+1];
	strcpy(this->email_from, email_from);
};
void EMailSender::setEMailPassword(const char* email_password){
	delete [] this->email_password;
	this->email_password = new char[strlen(email_password)+1];
	strcpy(this->email_password, email_password);
};

void EMailSender::setIsSecure(bool isSecure) {
	this->isSecure = isSecure;
}

EMailSender::Response EMailSender::awaitSMTPResponse(WiFiClientSecure &client,
		const char* resp, const char* respDesc, uint16_t timeOut) {
	EMailSender::Response response;
	uint32_t ts = millis();
	while (!client.available()) {
		if (millis() > (ts + timeOut)) {
			response.code = F("1");
			response.desc = F("SMTP Response TIMEOUT!");
			response.status = false;
			return response;
		}
	}
	_serverResponce = client.readStringUntil('\n');

	DEBUG_PRINTLN(_serverResponce);
	if (resp && _serverResponce.indexOf(resp) == -1){
		response.code = resp;
		response.desc = respDesc;
		response.status = false;
		return response;
	}

	response.status = true;
	return response;
}

EMailSender::Response EMailSender::send(const char* to, EMailMessage &email)
{
  WiFiClientSecure client;

  DEBUG_PRINT(F("Insecure client:"));
  DEBUG_PRINTLN(this->isSecure);

#ifndef ARDUINO_ESP8266_RELEASE_2_4_2
  if (this->isSecure == false){
	  client.setInsecure();
	  bool mfln = client.probeMaxFragmentLength(this->smtp_server, this->smtp_port, 512);

	  DEBUG_PRINT("MFLN supported: ");
	  DEBUG_PRINTLN(mfln?"yes":"no");

	  if (mfln) {
		  client.setBufferSizes(512, 512);
	  }
  }
#endif

  EMailSender::Response response;

  if(!client.connect(this->smtp_server, this->smtp_port)) {
	  response.desc = F("Could not connect to mail server");
	  response.code = F("2");
	  response.status = false;

	  return response;
  }

  response = awaitSMTPResponse(client, "220", "Connection Error");
  if (!response.status) return response;



  DEBUG_PRINTLN(F("HELO friend:"));
  client.println(F("HELO friend"));

  response = awaitSMTPResponse(client, "250", "Identification error");
  if (!response.status) return response;


  DEBUG_PRINTLN(F("AUTH LOGIN:"));
  client.println(F("AUTH LOGIN"));
  awaitSMTPResponse(client);

  DEBUG_PRINTLN(encode64(this->email_login));
  client.println(encode64(this->email_login));
  awaitSMTPResponse(client);

  DEBUG_PRINTLN(encode64(this->email_password));
  client.println(encode64(this->email_password));

  response = awaitSMTPResponse(client, "235", "SMTP AUTH error");
  if (!response.status) return response;

  String mailFrom = "MAIL FROM: <" + String(this->email_from) + '>';
  DEBUG_PRINTLN(mailFrom);
  client.println(mailFrom);
  awaitSMTPResponse(client);

  String rcpt = "RCPT TO: <" + String(to) + '>';

  DEBUG_PRINTLN(rcpt);
  client.println(rcpt);
  awaitSMTPResponse(client);

  DEBUG_PRINTLN(F("DATA:"));
  client.println(F("DATA"));

  response = awaitSMTPResponse(client, "354", "SMTP DATA error");
  if (!response.status) return response;

  client.println("From: <" + String(this->email_from) + '>');
  client.println("To: <" + String(to) + '>');

  client.print(F("Subject: "));
  client.println(email.subject);

  client.println(F("Mime-Version: 1.0"));
  client.println(F("Content-Type: text/html; charset=\"UTF-8\""));
  client.println(F("Content-Transfer-Encoding: 7bit"));
  client.println();
  String body = "<!DOCTYPE html><html lang=\"en\">" + String(email.message) + "</html>";
  client.println(body);
  client.println(".");

  response = awaitSMTPResponse(client, "250", "Sending message error");
  if (!response.status) return response;

  client.println(F("QUIT"));

  response = awaitSMTPResponse(client, "221", "SMTP QUIT error");
  if (!response.status) return response;

  response.status = true;
  response.code = F("0");
  response.desc = F("Message sent!");

  return response;
}

