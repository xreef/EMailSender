/*
 * EMail Sender Arduino, esp8266 and esp32 library to send email
 *
 * AUTHOR:  Renzo Mischianti
 * VERSION: 2.3.0
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

#include "EMailSender.h"
#include<stdio.h>
#include <lwip/err.h>
#include <lwip/sockets.h>
#include <lwip/sys.h>
#include <lwip/netdb.h>
#include <mbedtls/sha256.h>
#include <mbedtls/oid.h>
#include "mbedtls/ctr_drbg.h"

#include <algorithm>

//#include <SPIFFS.h>
//#include <LittleFS.h>

//#define SD SPIFFS

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

EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* name_from ,
		const char* smtp_server, uint16_t smtp_port) {
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);
	this->setSMTPServer(smtp_server);
	this->setSMTPPort(smtp_port);
	this->setNameFrom(name_from);
//	this->isSecure = isSecure;
}
EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from,
		const char* smtp_server, uint16_t smtp_port) {
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);
	this->setSMTPServer(smtp_server);
	this->setSMTPPort(smtp_port);

//	this->isSecure = isSecure;
}

EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from, const char* name_from ) {
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);
	this->setNameFrom(name_from);
	this->setNameFrom(name_from);

//	this->isSecure = isSecure;
}
EMailSender::EMailSender(const char* email_login, const char* email_password, const char* email_from) {
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_from);
	this->setEMailPassword(email_password);

//	this->isSecure = isSecure;
}

EMailSender::EMailSender(const char* email_login, const char* email_password){
	this->setEMailLogin(email_login);
	this->setEMailFrom(email_login);
	this->setEMailPassword(email_password);

//	this->isSecure = isSecure;
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
void EMailSender::setNameFrom(const char* name_from){
	delete [] this->name_from;
	this->name_from = new char[strlen(name_from)+1];
	strcpy(this->name_from, name_from);
};
void EMailSender::setEMailPassword(const char* email_password){
	delete [] this->email_password;
	this->email_password = new char[strlen(email_password)+1];
	strcpy(this->email_password, email_password);
};

void EMailSender::setIsSecure(bool isSecure) {
	this->isSecure = isSecure;
}

EMailSender::Response EMailSender::awaitSMTPResponse(EMAIL_NETWORK_SSL_CLASS &client,
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
		response.desc = respDesc +String(" (") + _serverResponce + String(")");
		response.status = false;
		return response;
	}

	response.status = true;
	return response;
}

EMailSender::Response EMailSender::awaitSMTPResponseInsecure(EMAIL_NETWORK_CLASS &client,
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
		response.desc = respDesc +String(" (") + _serverResponce + String(")");
		response.status = false;
		return response;
	}

	response.status = true;
	return response;
}

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void encodeblock(unsigned char in[3],unsigned char out[4],int len) {
 out[0]=cb64[in[0]>>2]; out[1]=cb64[((in[0]&0x03)<<4)|((in[1]&0xF0)>>4)];
 out[2]=(unsigned char) (len>1 ? cb64[((in[1]&0x0F)<<2)|((in[2]&0xC0)>>6)] : '=');
 out[3]=(unsigned char) (len>2 ? cb64[in[2]&0x3F] : '=');
}

#ifdef ENABLE_ATTACHMENTS
	#if (defined(STORAGE_INTERNAL_ENABLED) && defined(FS_NO_GLOBALS))
			void encode(fs::File *file, EMAIL_NETWORK_SSL_CLASS *client) {
			 unsigned char in[3],out[4];
			 int i,len,blocksout=0;

			 while (file->available()!=0) {
			   len=0;
				 for (i=0;i<3;i++){
					   in[i]=(unsigned char) file->read();
						   if (file->available()!=0) len++;
								 else in[i]=0;
				 }
				 if (len){
					 encodeblock(in,out,len);
			//         for(i=0;i<4;i++) client->write(out[i]);
					 client->write(out, 4);
					 blocksout++; }
				 if (blocksout>=19||file->available()==0){
					 if (blocksout) {
						 client->print("\r\n");
					 }
					 blocksout=0;
				 }
			  }
			}
	#endif

	#if (defined(STORAGE_SD_ENABLED) || (defined(STORAGE_INTERNAL_ENABLED) && !defined(FS_NO_GLOBALS)))
	void encode(File *file, EMAIL_NETWORK_SSL_CLASS *client) {
	 unsigned char in[3],out[4];
	 int i,len,blocksout=0;

	 while (file->available()!=0) {
	   len=0;
		 for (i=0;i<3;i++){
			   in[i]=(unsigned char) file->read();
				   if (file->available()!=0) len++;
						 else in[i]=0;
		 }
		 if (len){
			 encodeblock(in,out,len);
	//         for(i=0;i<4;i++) client->write(out[i]);
			 client->write(out, 4);
			 blocksout++; }
		 if (blocksout>=19||file->available()==0){
			 if (blocksout) {
				 client->print("\r\n");
			 }
			 blocksout=0;
		 }
	  }
	}
	#endif
#endif

const char** toCharArray(String arr[], int num) {
    // If we ever alloc with new with have to delete
    const char** buffer = new const char*[num];

    for(int i = 0; i < num; i++) {
        buffer[i] = arr[i].c_str();
    }

    return buffer;
}
const char** toCharArray(char* arr[], int num) {
    // If we ever alloc with new with have to delete
    const char** buffer = new const char*[num];

    for(int i = 0; i < num; i++) {
        buffer[i] = arr[i];
    }

    return buffer;
}

EMailSender::Response EMailSender::send(char* tos[], byte sizeOfTo, EMailMessage &email, Attachments attachments) {
	return send(toCharArray(tos, sizeOfTo), sizeOfTo, 0, 0, email, attachments);
}
EMailSender::Response EMailSender::send(char* tos[], byte sizeOfTo,  byte sizeOfCc,  EMailMessage &email, Attachments attachments) {
	return send(toCharArray(tos, sizeOfTo+sizeOfCc), sizeOfTo, sizeOfCc, 0, email, attachments);
}
EMailSender::Response EMailSender::send(char* tos[], byte sizeOfTo,  byte sizeOfCc,byte sizeOfCCn, EMailMessage &email, Attachments attachments){
	return send(toCharArray(tos, sizeOfTo+sizeOfCc+sizeOfCCn), sizeOfTo, sizeOfCc, sizeOfCCn, email, attachments);
}


EMailSender::Response EMailSender::send(String to, EMailMessage &email, Attachments attachments){
	  DEBUG_PRINT(F("ONLY ONE RECIPIENT"));

	const char* arrEmail[] =  {to.c_str()};
	return send(arrEmail, 1, email, attachments);
}

EMailSender::Response EMailSender::send(String tos[], byte sizeOfTo, EMailMessage &email, Attachments attachments) {
	return send(toCharArray(tos, sizeOfTo), sizeOfTo, 0, 0, email, attachments);
}

EMailSender::Response EMailSender::send(String tos[], byte sizeOfTo,  byte sizeOfCc,  EMailMessage &email, Attachments attachments) {
	return send(toCharArray(tos, sizeOfTo+sizeOfCc), sizeOfTo, sizeOfCc, 0, email, attachments);
}

EMailSender::Response EMailSender::send(String tos[], byte sizeOfTo,  byte sizeOfCc,byte sizeOfCCn, EMailMessage &email, Attachments attachments){
	return send(toCharArray(tos, sizeOfTo+sizeOfCc+sizeOfCCn), sizeOfTo, sizeOfCc, sizeOfCCn, email, attachments);
}

EMailSender::Response EMailSender::send(const char* to, EMailMessage &email, Attachments attachments){
	  DEBUG_PRINT(F("ONLY ONE RECIPIENT"));

	const char* arrEmail[] =  {to};
	return send(arrEmail, 1, email, attachments);
}

EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo, EMailMessage &email, Attachments attachments) {
	DEBUG_PRINTLN(F("miltiple destination and attachments"));
	return send(to, sizeOfTo, 0, email, attachments);
}

EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo,  byte sizeOfCc,  EMailMessage &email, Attachments attachments)
{
	return send(to, sizeOfTo, sizeOfCc, 0, email, attachments);
}

void ssl_client_debug_pgm_send_cb(PGM_P info)
{
    size_t dbgInfoLen = strlen_P(info) + 10;
    char *dbgInfo = new char[dbgInfoLen];
    memset(dbgInfo, 0, dbgInfoLen);
    strcpy_P(dbgInfo, info);
    DEBUG_PRINTLN(dbgInfo);
    delete[] dbgInfo;
}
static const char esp_ssl_client_str_1[] PROGMEM = "! E: ";
static const char esp_ssl_client_str_2[] PROGMEM = "> C: starting socket";
static const char esp_ssl_client_str_3[] PROGMEM = "! E: opening socket";
static const char esp_ssl_client_str_4[] PROGMEM = "! E: could not get ip from host";
static const char esp_ssl_client_str_5[] PROGMEM = "> C: connecting to Server";
static const char esp_ssl_client_str_6[] PROGMEM = "> C: server connected";
static const char esp_ssl_client_str_7[] PROGMEM = "! E: connect to Server failed!";
static const char esp_ssl_client_str_8[] PROGMEM = "< S: ";
static const char esp_ssl_client_str_9[] PROGMEM = "> C: seeding the random number generator";
static const char esp_ssl_client_str_10[] PROGMEM = "> C: setting up the SSL/TLS structure";
static const char esp_ssl_client_str_11[] PROGMEM = "> C: loading CA cert";
static const char esp_ssl_client_str_12[] PROGMEM = "> C: setting up PSK";
static const char esp_ssl_client_str_13[] PROGMEM = "! E: pre-shared key not valid hex or too long";
static const char esp_ssl_client_str_14[] PROGMEM = "> C: set mbedtls config";
static const char esp_ssl_client_str_15[] PROGMEM = "> C: loading CRT cert";
static const char esp_ssl_client_str_16[] PROGMEM = "> C: loading private key";
static const char esp_ssl_client_str_17[] PROGMEM = "> C: setting hostname for TLS session";
static const char esp_ssl_client_str_18[] PROGMEM = "> C: performing the SSL/TLS handshake";
static const char esp_ssl_client_str_19[] PROGMEM = "> C: verifying peer X.509 certificate";
static const char esp_ssl_client_str_20[] PROGMEM = "! E: failed to verify peer certificate!";
static const char esp_ssl_client_str_21[] PROGMEM = "> C: certificate verified";
static const char esp_ssl_client_str_22[] PROGMEM = "> C: cleaning SSL connection";
static const char esp_ssl_client_str_23[] PROGMEM = "! E: fingerprint too short";
static const char esp_ssl_client_str_24[] PROGMEM = "! E: invalid hex sequence";
static const char esp_ssl_client_str_25[] PROGMEM = "! E: could not fetch peer certificate";
static const char esp_ssl_client_str_26[] PROGMEM = "! E: fingerprint doesn't match";
static const char esp_ssl_client_str_27[] PROGMEM = "! E: root certificate, PSK identity or keys are required for secured connection";
static const char esp_ssl_client_str_28[] PROGMEM = "! W: Skipping SSL Verification. INSECURE!";


int start_socket(sslclient_context *ssl, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey, bool insecure)
{

    if (rootCABuff == NULL && pskIdent == NULL && psKey == NULL && !insecure)
    {
//        if (ssl->_debugCallback)
//            ssl_client_debug_pgm_send_cb(ssl, esp_ssl_client_str_27);
        return -1;
    }

    int enable = 1;

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb(esp_ssl_client_str_2);

    DEBUG_PRINT("Free internal heap before TLS %u");
    DEBUG_PRINTLN( ESP.getFreeHeap());

    DEBUG_PRINTLN("Starting socket");
    ssl->socket = -1;

    ssl->socket = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ssl->socket < 0)
    {
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_3);
        log_e("ERROR opening socket");
        return ssl->socket;
    }

    IPAddress srv((uint32_t)0);
    if (!WiFiGenericClass::hostByName(host, srv))
    {
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_4);
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = srv;
    serv_addr.sin_port = htons(port);

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb( esp_ssl_client_str_5);

    if (lwip_connect(ssl->socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
    {
        if (timeout <= 0)
        {
            timeout = 30000; // Milli seconds.
        }
        timeval so_timeout = {.tv_sec = timeout / 1000, .tv_usec = (timeout % 1000) * 1000};

#define ROE(x, msg)                                         \
    {                                                       \
        if (((x) < 0))                                      \
        {                                                   \
            log_e("LWIP Socket config of " msg " failed."); \
            return -1;                                      \
        }                                                   \
    }
        ROE(lwip_setsockopt(ssl->socket, SOL_SOCKET, SO_RCVTIMEO, &so_timeout, sizeof(so_timeout)), "SO_RCVTIMEO");
        ROE(lwip_setsockopt(ssl->socket, SOL_SOCKET, SO_SNDTIMEO, &so_timeout, sizeof(so_timeout)), "SO_SNDTIMEO");

        ROE(lwip_setsockopt(ssl->socket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(enable)), "TCP_NODELAY");
        ROE(lwip_setsockopt(ssl->socket, SOL_SOCKET, SO_KEEPALIVE, &enable, sizeof(enable)), "SO_KEEPALIVE");
    }
    else
    {
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_7);
        log_e("Connect to Server failed!");
        return -1;
    }

    fcntl(ssl->socket, F_SETFL, fcntl(ssl->socket, F_GETFL, 0) | O_NONBLOCK);

    return ssl->socket;
}
const char *custom_str = "esp32-tls";

void ssl_client_send_mbedtls_error_cb(int errNo)
{
    char *buf = new char[512];
    char *error_buf = new char[100];
    memset(buf, 0, 512);
    strcpy_P(buf, esp_ssl_client_str_1);
    mbedtls_strerror(errNo, error_buf, 100);
    strcat(buf, error_buf);
    DEBUG_PRINT(buf);DEBUG_PRINTLN(errNo);
    delete[] error_buf;
    delete[] buf;
}

/* Change to a number between 1 and 4 to debug the TLS connection */
#define DEBUG_LEVEL 4
#if DEBUG_LEVEL > 0
#include "mbedtls/debug.h"
#endif

static void my_debug(void *ctx, int level, const char *file, int line,
                     const char *str)
{
    const char *p, *basename;
    (void) ctx;

    /* Extract basename from file */
    for(p = basename = file; *p != '\0'; p++) {
        if(*p == '/' || *p == '\\') {
            basename = p + 1;
        }
    }

    Serial.printf("%s:%04d: |%d| %s", basename, line, level, str);
}

int start_ssl_client_cust(sslclient_context *ssl, const char *host, uint32_t port, int timeout, const char *rootCABuff, const char *cli_cert, const char *cli_key, const char *pskIdent, const char *psKey, bool insecure)
{
    mbedtls_ssl_conf_dbg(( mbedtls_ssl_config *)&ssl->ssl_ctx.conf, my_debug, NULL);
//    mbedtls_debug_set_threshold(DEBUG_LEVEL);

    char buf[512];
    int ret, flags;

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb( esp_ssl_client_str_9);

    DEBUG_PRINTLN("Seeding the random number generator");
    mbedtls_entropy_init(&ssl->entropy_ctx);

    ret = mbedtls_ctr_drbg_seed(&ssl->drbg_ctx, mbedtls_entropy_func, &ssl->entropy_ctx, (const unsigned char *)custom_str, strlen(custom_str));
    if (ret < 0)
    {
//        if (ssl->_debugCallback)
            ssl_client_send_mbedtls_error_cb( ret);
        return ret; //esp32_ssl_handle_error(ret);
    }

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb( esp_ssl_client_str_10);

    DEBUG_PRINTLN("Setting up the SSL/TLS structure...");

    if ((ret = mbedtls_ssl_config_defaults(&ssl->ssl_conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
//        if (ssl->_debugCallback)
            ssl_client_send_mbedtls_error_cb( ret);
        return ret; //esp32_ssl_handle_error(ret);
    }

    // MBEDTLS_SSL_VERIFY_REQUIRED if a CA certificate is defined on Arduino IDE and
    // MBEDTLS_SSL_VERIFY_NONE if not.

    if (insecure)
    {
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_28);

        mbedtls_ssl_conf_authmode(&ssl->ssl_conf, MBEDTLS_SSL_VERIFY_NONE);
        log_i("WARNING: Skipping SSL Verification. INSECURE!");
    }
    else if (rootCABuff != NULL)
    {
//        if (ssl->_debugCallback)
//            ssl_client_debug_pgm_send_cb(ssl, esp_ssl_client_str_11);
        DEBUG_PRINTLN("Loading CA cert");
        mbedtls_x509_crt_init(&ssl->ca_cert);
        mbedtls_ssl_conf_authmode(&ssl->ssl_conf, MBEDTLS_SSL_VERIFY_REQUIRED);
        ret = mbedtls_x509_crt_parse(&ssl->ca_cert, (const unsigned char *)rootCABuff, strlen(rootCABuff) + 1);
        mbedtls_ssl_conf_ca_chain(&ssl->ssl_conf, &ssl->ca_cert, NULL);
        //mbedtls_ssl_conf_verify(&ssl->ssl_ctx, my_verify, NULL );
        if (ret < 0)
        {
//            if (ssl->_debugCallback)
                ssl_client_send_mbedtls_error_cb( ret);
            // free the ca_cert in the case parse failed, otherwise, the old ca_cert still in the heap memory, that lead to "out of memory" crash.
            mbedtls_x509_crt_free(&ssl->ca_cert);
            return ret; // esp32_ssl_handle_error(ret);
        }
    }
    else if (pskIdent != NULL && psKey != NULL)
    {
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_12);
        DEBUG_PRINTLN("Setting up PSK");
        // convert PSK from hex to binary
        if ((strlen(psKey) & 1) != 0 || strlen(psKey) > 2 * MBEDTLS_PSK_MAX_LEN)
        {
//            if (ssl->_debugCallback)
                ssl_client_debug_pgm_send_cb( esp_ssl_client_str_13);
            log_e("pre-shared key not valid hex or too long");
            return -1;
        }

        unsigned char psk[MBEDTLS_PSK_MAX_LEN];
        size_t psk_len = strlen(psKey) / 2;
        for (int j = 0; j < strlen(psKey); j += 2)
        {
            char c = psKey[j];
            if (c >= '0' && c <= '9')
                c -= '0';
            else if (c >= 'A' && c <= 'F')
                c -= 'A' - 10;
            else if (c >= 'a' && c <= 'f')
                c -= 'a' - 10;
            else
                return -1;
            psk[j / 2] = c << 4;
            c = psKey[j + 1];
            if (c >= '0' && c <= '9')
                c -= '0';
            else if (c >= 'A' && c <= 'F')
                c -= 'A' - 10;
            else if (c >= 'a' && c <= 'f')
                c -= 'a' - 10;
            else
                return -1;
            psk[j / 2] |= c;
        }
        // set mbedtls config
//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_14);

        ret = mbedtls_ssl_conf_psk(&ssl->ssl_conf, psk, psk_len,
                                   (const unsigned char *)pskIdent, strlen(pskIdent));
        if (ret != 0)
        {
//            if (ssl->_debugCallback)
                ssl_client_send_mbedtls_error_cb( ret);

            log_e("mbedtls_ssl_conf_psk returned %d", ret);
            return ret; //esp32_ssl_handle_error(ret);
        }
    }
    else
    {
        return -1;
    }

    if (!insecure && cli_cert != NULL && cli_key != NULL)
    {

        mbedtls_x509_crt_init(&ssl->client_cert);
        mbedtls_pk_init(&ssl->client_key);

//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_15);

        DEBUG_PRINTLN("Loading CRT cert");

        ret = mbedtls_x509_crt_parse(&ssl->client_cert, (const unsigned char *)cli_cert, strlen(cli_cert) + 1);
        if (ret < 0)
        {
//            if (ssl->_debugCallback)
                ssl_client_send_mbedtls_error_cb( ret);
            // free the client_cert in the case parse failed, otherwise, the old client_cert still in the heap memory, that lead to "out of memory" crash.
            mbedtls_x509_crt_free(&ssl->client_cert);
            return ret; // esp32_ssl_handle_error(ret);
        }

//        if (ssl->_debugCallback)
            ssl_client_debug_pgm_send_cb( esp_ssl_client_str_16);

        DEBUG_PRINTLN("Loading private key");
        ret = mbedtls_pk_parse_key(&ssl->client_key, (const unsigned char *)cli_key, strlen(cli_key) + 1, NULL, 0);

        if (ret != 0)
        {
//            if (ssl->_debugCallback)
                ssl_client_send_mbedtls_error_cb( ret);
            return ret; //esp32_ssl_handle_error(ret);
        }

        mbedtls_ssl_conf_own_cert(&ssl->ssl_conf, &ssl->client_cert, &ssl->client_key);
    }

    delay(100);
//    if (ssl->_debugCallback)
	ssl_client_debug_pgm_send_cb( esp_ssl_client_str_17);

    DEBUG_PRINTLN("Setting hostname for TLS session...");

    // Hostname set here should match CN in server certificate
    if ((ret = mbedtls_ssl_set_hostname(&ssl->ssl_ctx, host)) != 0)
    {
//        if (ssl->_debugCallback)
            ssl_client_send_mbedtls_error_cb( ret);
        return ret; // esp32_ssl_handle_error(ret);
    }

    DEBUG_PRINTLN("Setting hostname for TLS session...OK");


    mbedtls_ssl_conf_rng(&ssl->ssl_conf, mbedtls_ctr_drbg_random, &ssl->drbg_ctx);

    if ((ret = mbedtls_ssl_setup(&ssl->ssl_ctx, &ssl->ssl_conf)) != 0)
    {
//        if (ssl->_debugCallback)
            ssl_client_send_mbedtls_error_cb( ret);
        return ret; // esp32_ssl_handle_error(ret);
    }

    mbedtls_ssl_set_bio(&ssl->ssl_ctx, &ssl->socket, mbedtls_net_send, mbedtls_net_recv, NULL);

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb( esp_ssl_client_str_18);

    DEBUG_PRINTLN("Performing the SSL/TLS handshake...");
    unsigned long handshake_start_time = millis();
    while ((ret = mbedtls_ssl_handshake(&ssl->ssl_ctx)) != 0)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
//            if (ssl->_debugCallback)
        	DEBUG_PRINT("NEXT HANDSHAKE");
                ssl_client_send_mbedtls_error_cb( ret);
            return ret; //esp32_ssl_handle_error(ret);
        }
        if ((millis() - handshake_start_time) > ssl->handshake_timeout)
            return -1;
        vTaskDelay(2); //2 ticks
    }

    if (cli_cert != NULL && cli_key != NULL)
    {
    	DEBUG_PRINT("Protocol is %s Ciphersuite is %s"); DEBUG_PRINT(mbedtls_ssl_get_version(&ssl->ssl_ctx)); DEBUG_PRINTLN( mbedtls_ssl_get_ciphersuite(&ssl->ssl_ctx));
        if ((ret = mbedtls_ssl_get_record_expansion(&ssl->ssl_ctx)) >= 0)
        {
            log_d("Record expansion is %d", ret);
        }
        else
        {
            log_w("Record expansion is unknown (compression)");
        }
    }

//    if (ssl->_debugCallback)
        ssl_client_debug_pgm_send_cb( esp_ssl_client_str_19);

    DEBUG_PRINTLN("Verifying peer X.509 certificate...");

    if ((flags = mbedtls_ssl_get_verify_result(&ssl->ssl_ctx)) != 0)
    {
        memset(buf, 0, sizeof(buf));
        mbedtls_x509_crt_verify_info(buf, sizeof(buf), "  ! ", flags);
        log_e("Failed to verify peer certificate! verification info: %s", buf);
        stop_ssl_socket(ssl, rootCABuff, cli_cert, cli_key); //It's not safe continue.
        return ret; // esp32_ssl_handle_error(ret);
    }
    else
    {
        DEBUG_PRINTLN("Certificate verified.");
    }

    if (rootCABuff != NULL)
    {
        mbedtls_x509_crt_free(&ssl->ca_cert);
    }

    if (cli_cert != NULL)
    {
        mbedtls_x509_crt_free(&ssl->client_cert);
    }

    if (cli_key != NULL)
    {
        mbedtls_pk_free(&ssl->client_key);
    }

    DEBUG_PRINT("Free internal heap after TLS %u");
    DEBUG_PRINTLN(ESP.getFreeHeap());

    return ssl->socket;
}


EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo,  byte sizeOfCc,byte sizeOfCCn, EMailMessage &email, Attachments attachments)
{
	EMAIL_NETWORK_SSL_CLASS client;
//	SSLClient client(base_client, TAs, (size_t)TAs_NUM, A5);

  DEBUG_PRINT(F("Insecure client:"));
  DEBUG_PRINTLN(this->isSecure);

#if (EMAIL_NETWORK_TYPE == NETWORK_ESP8266 || EMAIL_NETWORK_TYPE == NETWORK_ESP8266_242)
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
#elif (EMAIL_NETWORK_TYPE == NETWORK_ESP32)
//	  String coreVersion = String(ESP.getSdkVersion());
//	  uint8_t firstdot = coreVersion.indexOf('.');
//
//	  DEBUG_PRINTLN(coreVersion.substring(1, coreVersion.indexOf('.', firstdot+1)).toFloat());
//	  DEBUG_PRINTLN(coreVersion.substring(1, coreVersion.indexOf('.', firstdot+1)).toFloat() >= 3.3f);
//	  if (coreVersion.substring(1, coreVersion.indexOf('.', firstdot+1)).toFloat() >= 3.3f) {
//		  client.setInsecure();
//	  }
	#include <core_version.h>
	#if ((!defined(ARDUINO_ESP32_RELEASE_1_0_4)) && (!defined(ARDUINO_ESP32_RELEASE_1_0_3)) && (!defined(ARDUINO_ESP32_RELEASE_1_0_2)))
		  client.setInsecure();
	#endif
//	client.setInsecure();
//	  DEBUG_PRINTLN(F("Insecure client:"));

#endif

  EMailSender::Response response;

  DEBUG_PRINTLN(this->smtp_server);
  DEBUG_PRINTLN(this->smtp_port);

  if (this->smtp_port==587){
	  EMAIL_NETWORK_CLASS basicClient;
	  if(!basicClient.connect(this->smtp_server, this->smtp_port)) {
		  response.desc = F("Could not connect to mail server");
		  response.code = F("2");
		  response.status = false;

		  return response;
	  }

	  basicClient.setTimeout(30);

	  response = awaitSMTPResponseInsecure(basicClient, "220", "Connection Error");
	  if (!response.status) return response;

	  String commandHELO = "HELO";
	  String helo = commandHELO; // + " "+String(publicIPDescriptor)+" ";
	  DEBUG_PRINTLN(helo);
	  basicClient.println(helo);

	  response = awaitSMTPResponseInsecure(basicClient, "250", "Identification error");
	  if (!response.status) return response;

	  DEBUG_PRINTLN(F("STARTTLS"));
	  basicClient.println(F("STARTTLS"));

	  response = awaitSMTPResponseInsecure(basicClient, "220", "Server not ready!");
	  if (!response.status) return response;

	  DEBUG_PRINTLN(F("SSL HAND"));

	    sslclient_context *sslclient;

		sslclient = new sslclient_context;
		ssl_init(sslclient);
		sslclient->socket = 0;
		sslclient->handshake_timeout = 120000;

		  DEBUG_PRINTLN(F("SSL HAND 2"));

		const char* pskIdent;
		const char* psKey;

		basicClient.stop();

		sslclient->socket = start_socket(sslclient, this->smtp_server, this->smtp_port, sslclient->handshake_timeout, NULL, NULL, NULL,pskIdent, psKey,true);

		int ret = start_ssl_client_cust(sslclient, this->smtp_server, this->smtp_port, sslclient->handshake_timeout, NULL, NULL, NULL,pskIdent, psKey,true);

		  DEBUG_PRINTLN(F("SSL HAND 4"));

		DEBUG_PRINT(F("startesp32_ssl_client: "));
        DEBUG_PRINTLN(ret);

//  	  String commandHELO = "HELO";
//  	  String helo = commandHELO; // + " "+String(publicIPDescriptor)+" ";
  	  DEBUG_PRINTLN("HELO");
  	  basicClient.println("HELO");

  	  response = awaitSMTPResponseInsecure(basicClient, "250", "Identification error");
  	  if (!response.status) return response;


  }


  if(!client.connect(this->smtp_server, this->smtp_port)) {
	  response.desc = F("Could not connect to mail server");
	  response.code = F("2");
	  response.status = false;

	  return response;
  }

  response = awaitSMTPResponse(client, "220", "Connection Error");
  if (!response.status) return response;

  String commandHELO = "HELO";
  if (this->useEHLO == true) {
	  commandHELO = "EHLO";
  }
  String helo = commandHELO + " "+String(publicIPDescriptor)+": ";
  DEBUG_PRINTLN(helo);
  client.println(helo);

  response = awaitSMTPResponse(client, "250", "Identification error");
  if (!response.status) return response;

  if (this->useEHLO == true) {
	  for (int i = 0; i<=6; i++) awaitSMTPResponse(client);
  }

  if (useAuth){
	 if (this->isSASLLogin == true){

		  int size = 1 + strlen(this->email_login)+ strlen(this->email_password)+2;
	      char * logPass = (char *) malloc(size);

//	      strcpy(logPass, " ");
//	      strcat(logPass, this->email_login);
//	      strcat(logPass, " ");
//	      strcat(logPass, this->email_password);

//		  String logPass;
	      int maincont = 0;

	      logPass[maincont++] = ' ';
	      logPass[maincont++] = (char) 0;

	      for (unsigned int i = 0;i<strlen(this->email_login);i++){
	    	  logPass[maincont++] = this->email_login[i];
	      }
	      logPass[maincont++] = (char) 0;
	      for (unsigned int i = 0;i<strlen(this->email_password);i++){
	    	  logPass[maincont++] = this->email_password[i];
	      }


//	      strcpy(logPass, "\0");
//	      strcat(logPass, this->email_login);
//	      strcat(logPass, "\0");
//	      strcat(logPass, this->email_password);

		  String auth = "AUTH PLAIN "+String(encode64_f(logPass, size));
//		  String auth = "AUTH PLAIN "+String(encode64(logPass));
		  DEBUG_PRINTLN(auth);
		  client.println(auth);
	  }else{
		  DEBUG_PRINTLN(F("AUTH LOGIN:"));
		  client.println(F("AUTH LOGIN"));
		  awaitSMTPResponse(client);

		  DEBUG_PRINTLN(encode64(this->email_login));
		  client.println(encode64(this->email_login));
		  awaitSMTPResponse(client);

		  DEBUG_PRINTLN(encode64(this->email_password));
		  client.println(encode64(this->email_password));
	  }
	  response = awaitSMTPResponse(client, "235", "SMTP AUTH error");
	  if (!response.status) return response;
  }
  DEBUG_PRINT(F("MAIL FROM: <"));
  DEBUG_PRINT(this->email_from);
  DEBUG_PRINTLN(F(">"));

  client.print(F("MAIL FROM: <"));
  client.print(this->email_from);
  client.println(F(">"));
  awaitSMTPResponse(client);

//  String rcpt = "RCPT TO: <" + String(to) + '>';
//
//  DEBUG_PRINTLN(rcpt);
//  client.println(rcpt);

  int cont;
  for (cont=0;cont<(sizeOfTo+sizeOfCc+sizeOfCCn);cont++){
	  DEBUG_PRINT(F("RCPT TO: <"));
	  DEBUG_PRINT(to[cont]);
	  DEBUG_PRINTLN(F(">"));

	  client.print(F("RCPT TO: <"));
	  client.print(to[cont]);
	  client.println(F(">"));
	  awaitSMTPResponse(client);
  }

  DEBUG_PRINTLN(F("DATA:"));
  client.println(F("DATA"));

  response = awaitSMTPResponse(client, "354", "SMTP DATA error");
  if (!response.status) return response;

//  client.println("From: <" + String(this->email_from) + '>');

  client.print(F("From: "));
  if (this->name_from){
	  client.print(this->name_from);
  }
  client.print(F(" <"));
  client.print(this->email_from);
  client.println(F(">"));

//  client.println("To: <" + String(to) + '>');

  client.print(F("To: "));
  for (cont=0;cont<sizeOfTo;cont++){
	  client.print(F("<"));
	  client.print(to[cont]);
	  client.print(">");
	  if (cont!=sizeOfTo-1){
		  client.print(",");
	  }
  }
  client.println();

  if (sizeOfCc>0){
	  client.print(F("Cc: "));
	  for (;cont<sizeOfTo+sizeOfCc;cont++){
		  client.print(F("<"));
		  client.print(to[cont]);
		  client.print(">");
		  if (cont!=sizeOfCc-1){
			  client.print(",");
		  }
	  }
	  client.println();
  }

  if (sizeOfCCn>0){
	  client.print(F("CCn: "));
	  for (;cont<sizeOfTo+sizeOfCc+sizeOfCCn;cont++){
		  client.print(F("<"));
		  client.print(to[cont]);
		  client.print(">");
		  if (cont!=sizeOfCCn-1){
			  client.print(",");
		  }
	  }
	  client.println();
  }

  client.print(F("Subject: "));
  client.println(email.subject);

//  client.println(F("Mime-Version: 1.0"));

  client.println(F("MIME-Version: 1.0"));
  client.println(F("Content-Type: Multipart/mixed; boundary=frontier"));

  client.println(F("--frontier"));

    client.print(F("Content-Type: "));
    client.print(email.mime);
    client.println(F("; charset=\"UTF-8\""));
//  client.println(F("Content-Type: text/html; charset=\"UTF-8\""));
  client.println(F("Content-Transfer-Encoding: 7bit"));
  client.println();
  if (email.mime==F("text/html")){
//	  String body = "<!DOCTYPE html><html lang=\"en\">" + String(email.message) + "</html>";

	  client.print(F("<!DOCTYPE html><html lang=\"en\">"));
	  client.print(email.message);
	  client.println(F("</html>"));

//	  client.println(body);
  }else{
	  client.println(email.message);
  }
  client.println();

#ifdef STORAGE_INTERNAL_ENABLED
  bool spiffsActive = false;
#endif
#ifdef STORAGE_SD_ENABLED
  bool sdActive = false;
#endif

#if defined(ENABLE_ATTACHMENTS) && (defined(STORAGE_SD_ENABLED) || defined(STORAGE_INTERNAL_ENABLED))
//  if ((sizeof(attachs) / sizeof(attachs[0]))>0){
  if (sizeof(attachments)>0 && attachments.number>0){

	  DEBUG_PRINT(F("Array: "));
//	  for (int i = 0; i<(sizeof(attachs) / sizeof(attachs[0])); i++){
	  for (int i = 0; i<attachments.number; i++){
		  uint8_t tBuf[64];

		  if (attachments.fileDescriptor[i].url.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no filename specified for the file "+attachments.fileDescriptor[i].filename;
			  response.status = false;
			  return response;
		  }
		  if (attachments.fileDescriptor[i].mime.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no mime type specified for the file "+attachments.fileDescriptor[i].url;
			  response.status = false;
			  return response;
		  }
		  if (attachments.fileDescriptor[i].filename.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no filename specified for the file "+attachments.fileDescriptor[i].url;
			  response.status = false;
			  return response;
		  }

		  DEBUG_PRINTLN(attachments.fileDescriptor[i].filename);
		  DEBUG_PRINTLN(F("--frontier"));
		  client.println(F("--frontier"));
		  DEBUG_PRINTLN(F("Content-Type: "));
		  client.print(F("Content-Type: "));
		  DEBUG_PRINTLN(attachments.fileDescriptor[i].mime);
		  client.print(attachments.fileDescriptor[i].mime);
		  DEBUG_PRINTLN(F("; charset=\"UTF-8\""));
		  client.println(F("; charset=\"UTF-8\""));

		  if (attachments.fileDescriptor[i].contentTransferEncoding.length()>0){
			  client.print(F("Content-Transfer-Encoding: "));
			  client.println(attachments.fileDescriptor[i].contentTransferEncoding);
		  } else if (attachments.fileDescriptor[i].encode64){
			  client.println(F("Content-Transfer-Encoding: base64"));
		  }

		  client.print(F("Content-Disposition: attachment; filename="));
		  client.print(attachments.fileDescriptor[i].filename);
		  client.println(F("\n"));

			int clientCount = 0;

#ifdef STORAGE_INTERNAL_ENABLED
			if (attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SPIFFS ||
				attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_LITTLE_FS ||
				attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_FFAT){
#ifdef OPEN_CLOSE_INTERNAL
				if (!INTERNAL_STORAGE_CLASS.exists(attachments.fileDescriptor[i].url)){
					if(!INTERNAL_STORAGE_CLASS.begin()){
						  EMailSender::Response response;
						  response.code = F("500");
						  response.desc = F("Error on startup filesystem!");
						  response.status = false;
						  return response;
					}

					spiffsActive = true;
					DEBUG_PRINTLN("SPIFFS BEGIN, ACTIVE");
				}
#endif

				fs::File myFile = INTERNAL_STORAGE_CLASS.open(attachments.fileDescriptor[i].url, "r");
				  if(myFile) {
//					  DEBUG_PRINTLN(myFile.name());
					  if (attachments.fileDescriptor[i].encode64){
						  encode(&myFile, &client);
					  }else{
						while(myFile.available()) {
						clientCount = myFile.read(tBuf,64);
						DEBUG_PRINTLN(clientCount);
						  client.write((byte*)tBuf,clientCount);
						}
					  }
					myFile.close();

					client.println();
				  }
				  else {
					  EMailSender::Response response;
					  response.code = F("404");
					  response.desc = "Error opening attachments file "+attachments.fileDescriptor[i].url;
					  response.status = false;
					  return response;
				  }

			}
#endif
#ifdef STORAGE_SD_ENABLED
			if (attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SD){
//				File myFile = SD.open(attachments.fileDescriptor[i].url, "r");
//				  if(myFile) {
//					while(myFile.available()) {
//					clientCount = myFile.read(tBuf,64);
//					  client.write((byte*)tBuf,clientCount);
//					}
//					myFile.close();
//				  }
//				  else {
//					  EMailSender::Response response;
//					  response.code = "404";
//					  response.desc = "Error opening attachments file "+attachments.fileDescriptor[i].url;
//					  response.status = false;
//					  return response;
//				  }
#ifdef OPEN_CLOSE_SD
				 DEBUG_PRINTLN(F("SD Check"));
				 if (!SD.exists(attachments.fileDescriptor[i].url.c_str())){
					if(!SD.begin(SD_CS_PIN)){
						  response.code = F("500");
						  response.desc = F("Error on startup SD filesystem!");
						  response.status = false;
						  return response;
					}
					sdActive = true;
				 }
#endif

			    DEBUG_PRINTLN(F("Open file: "));
			File myFile = SD.open(attachments.fileDescriptor[i].url.c_str());
			  if(myFile) {
				  myFile.seek(0);
				  DEBUG_PRINTLN(F("OK"));
				  if (attachments.fileDescriptor[i].encode64){
					  DEBUG_PRINTLN(F("BASE 64"));
					  encode(&myFile, &client);
				  }else{
					  DEBUG_PRINTLN(F("NORMAL"));
					while(myFile.available()) {
						clientCount = myFile.read(tBuf,64);
						client.write((byte*)tBuf,clientCount);
					}
				  }
				myFile.close();

				client.println();
			  }
			  else {
				  response.code = F("404");
				  response.desc = "Error opening attachments file "+attachments.fileDescriptor[i].url;
				  response.status = false;
				  return response;
			  }

			}
#endif

	  }
	  client.println();
	  client.println(F("--frontier--"));
#ifdef STORAGE_SD_ENABLED
	  #ifdef OPEN_CLOSE_SD
		  if (sdActive){
			  DEBUG_PRINTLN(F("SD end"));
#ifndef ARDUINO_ESP8266_RELEASE_2_4_2
			  SD.end();
#endif
			  DEBUG_PRINTLN(F("SD end 2"));
		  }
	#endif
#endif

#ifdef STORAGE_INTERNAL_ENABLED
	#ifdef OPEN_CLOSE_INTERNAL
		  if (spiffsActive){
			  INTERNAL_STORAGE_CLASS.end();
			  DEBUG_PRINTLN(F("SPIFFS END"));
		  }
	#endif
#endif

  }

#endif

  DEBUG_PRINTLN(F("Message end"));
  client.println(F("."));

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

