#include "EMailSender.h"

#if defined(EMAIL_ENABLE_OPENSLAB_SSLCLIENT)
#include "sslclientosu/OSUSSLClientAdapter.h"
#include "trust_anchors.h"
#endif

#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)
#include "sslclient/SSLClient.h"
#endif
#include <stdio.h>
#if defined(ESP32)
#include <mbedtls/base64.h>
#include <mbedtls/md.h>
#endif

#if (defined(ESP32) || defined(ESP8266)) && defined(MANAGE_DATE_HEADER)
#include <time.h>
#endif

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

	EMAIL_SENDER_DEBUG_PRINTLN(F("Encoding"));

	EMAIL_SENDER_DEBUG_PRINTLN(input);
	EMAIL_SENDER_DEBUG_PRINTLN(len);

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

void EMailSender::setClientType(ClientType type) {
    clientType = type;
#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)
    if (clientType == CLIENT_SSLCLIENT && sslClient == nullptr) {
        static EMAIL_TCP_BASE_CLIENT base_client;
        static sslclient::SSLClient static_sslclient(base_client);
        static_sslclient.setInsecure();
        sslClient = &static_sslclient;
    }
#else
    // Wrapper SSLClient disabled: force standard
    clientType = CLIENT_STANDARD;
#endif
}

// Helper: read one line (ending with \n) with timeout, independent from available()
static bool readLineWithTimeout(Client &client, String &line, uint16_t timeOut) {
    line = "";
    uint32_t ts = millis();
    while (millis() - ts < timeOut) {
        // ✅ Prima prova a leggere se available() indica dati
        while (client.available() > 0) {
            int b = client.read();
            if (b >= 0) {
                char ch = (char)b;
                line += ch;
                if (ch == '\n') return true;
            }
        }

        // ✅ Anche se available() == 0, prova comunque a leggere
        // Questo è necessario per SSL dove i dati potrebbero essere nel buffer interno mbedTLS
        int b = client.read();
        if (b >= 0) {
            char ch = (char)b;
            line += ch;
            if (ch == '\n') return true;
        } else {
            // Nessun dato disponibile, aspetta un po'
            #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || defined(__AVR_ATmega2560__)
                delay(10);  // Arduino Mega/AVR: 10ms per Ethernet lento
            #else
                delay(5);   // ESP32/ESP8266/altri: 5ms sufficiente
            #endif
        }
    }
    // Timeout: return true if something was read (partial line), otherwise false
    return line.length() > 0;
}

// Unified: always operates on a generic Client&
EMailSender::Response EMailSender::awaitSMTPResponse(Client &client,
		const char* resp, const char* respDesc, uint16_t timeOut) {
	EMailSender::Response response;
	String line;
	bool ok = readLineWithTimeout(client, line, timeOut);
	if (!ok) {
		response.code = F("1");
		response.desc = String(respDesc) + "! " + F("SMTP Response TIMEOUT!");
		response.status = false;
		return response;
	}
	_serverResponce = line;
	EMAIL_SENDER_DEBUG_PRINTLN(_serverResponce);
	if (resp && _serverResponce.indexOf(resp) == -1){
		response.code = resp;
		response.desc = respDesc + String(" (") + _serverResponce + String(")");
		response.status = false;
		return response;
	}
	response.status = true;
	return response;
}

// Drain multiline responses (e.g. EHLO: 250- ... 250 ...)
EMailSender::Response EMailSender::awaitSMTPResponseDrain(Client &client,
        const char* resp, const char* respDesc, uint16_t timeOut, uint8_t maxLines) {
    EMailSender::Response response = awaitSMTPResponse(client, resp, respDesc, timeOut);
    if (!response.status) return response;

    if (!(resp && String(resp) == String("250"))) return response;

    for (uint8_t i = 0; i < maxLines; i++) {
        String line;
        bool ok = readLineWithTimeout(client, line, timeOut);
        if (!ok) break; // no more lines
        _serverResponce = line;
        EMAIL_SENDER_DEBUG_PRINTLN(line);
        if (line.startsWith("250-")) continue; // still 250- lines
        if (line.startsWith("250 ")) break;    // last 250 <space> line
        // Different response: stop here and keep it in _serverResponce
        break;
    }
    response.status = true;
    response.code = F("250");
    return response;
}

static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
void encodeblock(unsigned char in[3],unsigned char out[4],int len) {
 out[0]=cb64[in[0]>>2];
 out[1]=cb64[((in[0]&0x03)<<4)|((in[1]&0xF0)>>4)];
 out[2]=(unsigned char) (len>1 ? cb64[((in[1]&0x0F)<<2)|((in[2]&0xC0)>>6)] : '=');
 out[3]=(unsigned char) (len>2 ? cb64[in[2]&0x3F] : '=');
}

#ifdef ENABLE_ATTACHMENTS
	#ifdef STORAGE_EXTERNAL_ENABLED
		#if (defined(DIFFERENT_FILE_MANAGE) && defined(EMAIL_FILE_EX)) || !defined(STORAGE_INTERNAL_ENABLED)
			// Uso sempre Client* per compatibilità universale
			void encode(EMAIL_FILE_EX *file, Client *client) {
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
	#ifdef STORAGE_INTERNAL_ENABLED
		#if defined(DIFFERENT_FILE_MANAGE) || (!defined(DIFFERENT_FILE_MANAGE) && defined(EMAIL_FILE)) || !defined(STORAGE_EXTERNAL_ENABLED)

			void encode(EMAIL_FILE *file, Client *client) {
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
#endif

// NEW: Base64 encoding function for Stream attachments
void encode(Stream *stream, Client *client) {
	unsigned char in[3], out[4];
	int i, len, blocksout = 0;

	while (stream->available() > 0) {
		len = 0;
		for (i = 0; i < 3; i++) {
			if (stream->available() > 0) {
				in[i] = (unsigned char)stream->read();
				len++;
			} else {
				in[i] = 0;
			}
		}
		if (len) {
			encodeblock(in, out, len);
			client->write(out, 4);
			blocksout++;
		}
		if (blocksout >= 19 || stream->available() == 0) {
			if (blocksout) {
				client->print("\r\n");
			}
			blocksout = 0;
		}
	}
}

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
	  EMAIL_SENDER_DEBUG_PRINT(F("ONLY ONE RECIPIENT"));

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
	  EMAIL_SENDER_DEBUG_PRINT(F("ONLY ONE RECIPIENT"));

	const char* arrEmail[] =  {to};
	return send(arrEmail, 1, email, attachments);
}

EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo, EMailMessage &email, Attachments attachments) {
	EMAIL_SENDER_DEBUG_PRINTLN(F("miltiple destination and attachments"));
	return send(to, sizeOfTo, 0, email, attachments);
}

EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo,  byte sizeOfCc,  EMailMessage &email, Attachments attachments)
{
	return send(to, sizeOfTo, sizeOfCc, 0, email, attachments);
}

#ifdef EMAIL_ENABLE_EXTERNAL_SSLCLIENT_OPENSLAB
#ifdef PUT_OUTSIDE_SCOPE_CLIENT_DECLARATION
	// Initialize the SSL client library
	// We input an EthernetClient, our trust anchors, and the analog pin
	EMAIL_NETWORK_CLASS base_client;
	SSLClient client(base_client, TAs, (size_t)TAs_NUM, ANALOG_PIN, 2);
#else
	#error "You must put outside scope the client declaration if you want use SSLClient!"
#endif
#else
	#ifdef PUT_OUTSIDE_SCOPE_CLIENT_DECLARATION
		EMAIL_NETWORK_CLASS client;
	#endif
#endif

EMailSender::Response EMailSender::send(const char* to[], byte sizeOfTo,  byte sizeOfCc,byte sizeOfCCn, EMailMessage &email, Attachments attachments)
{
    EMailSender::Response response;

    // Debug: print server and port
    EMAIL_SENDER_DEBUG_PRINTLN(this->smtp_server);
    EMAIL_SENDER_DEBUG_PRINTLN(this->smtp_port);

    // Select active client to use for SMTP dialogue
    Client* activeClient = nullptr;

    // Local instance for standard clients (WiFiClient/WiFiClientSecure/EthernetClient)
    EMAIL_NETWORK_CLASS localClient;

#if defined(EMAIL_ENABLE_OPENSLAB_SSLCLIENT)
    // Optional OPEnSLab SSLClient (BearSSL) backend for implicit TLS (465) over Ethernet/UIP/etc.
    // Not for STARTTLS (587)
    bool useStartTLS = (this->smtp_port == 587);
    if (!useStartTLS) {
        static EMAIL_NETWORK_CLASS base_client_openslab;
        static sslclientosu::OsuSSLClient openslab_sslclient(base_client_openslab, TAs, (size_t)TAs_NUM, ANALOG_PIN);
        bool connected = openslab_sslclient.connect(this->smtp_server, this->smtp_port);
        if (!connected) {
            response.desc = F("Could not connect to mail server (OPEnSLab SSLClient)");
            response.code = F("2");
            response.status = false;
            openslab_sslclient.stop();
            return response;
        }
        response = awaitSMTPResponse(openslab_sslclient, "220", "Connection Error");
        if (!response.status) {
            openslab_sslclient.stop();
            return response;
        }
        String commandHELO = this->useEHLO ? "EHLO" : "HELO";
        String helo = commandHELO + " "+String(publicIPDescriptor)+" ";
        EMAIL_SENDER_DEBUG_PRINTLN(helo);
        openslab_sslclient.println(helo);
        if (this->useEHLO) {
            response = awaitSMTPResponseDrain(openslab_sslclient, "250", "Identification error");
        } else {
            response = awaitSMTPResponse(openslab_sslclient, "250", "Identification error");
        }
        if (!response.status) {
            openslab_sslclient.stop();
            return response;
        }
        activeClient = &openslab_sslclient;
    }
    else
#endif
    {
        // Standard client path (WiFiClient/WiFiClientSecure/EthernetClient depending on target)

#if defined(ESP32) || defined(ESP8266)
        bool useSTARTTLS = (this->smtp_port == 587);

        if (useSTARTTLS) {
#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)
            // Usa sslclient::SSLClient per STARTTLS sulla porta 587
            EMAIL_SENDER_DEBUG_PRINTLN(F("Using SSLClient with STARTTLS for port 587"));

            static WiFiClient baseClient;
            static sslclient::SSLClient sslClient(baseClient);
            sslClient.setInsecure();

            // Connessione iniziale non sicura
            bool connected = baseClient.connect(this->smtp_server, this->smtp_port);
            if (!connected) {
                IPAddress ip;
                if (WiFi.hostByName(this->smtp_server, ip)) {
                    EMAIL_SENDER_DEBUG_PRINT(F("Resolved IP: "));
                    EMAIL_SENDER_DEBUG_PRINTLN(ip);
                    connected = baseClient.connect(ip, this->smtp_port);
                }
            }

            if (!connected) {
                response.desc = F("Could not connect to mail server");
                response.code = F("2");
                response.status = false;
                baseClient.stop();
                return response;
            }

            // Ricevi il banner iniziale
            response = awaitSMTPResponse(baseClient, "220", "Connection Error");
            if (!response.status) {
                baseClient.flush();
                baseClient.stop();
                return response;
            }

            // Invia EHLO
            String commandHELO = this->useEHLO ? "EHLO" : "HELO";
            String helo = commandHELO + " "+String(publicIPDescriptor)+" ";
            EMAIL_SENDER_DEBUG_PRINTLN(helo);
            baseClient.println(helo);

            if (this->useEHLO) {
                response = awaitSMTPResponseDrain(baseClient, "250", "Identification error");
            } else {
                response = awaitSMTPResponse(baseClient, "250", "Identification error");
            }
            if (!response.status) {
                baseClient.flush();
                baseClient.stop();
                return response;
            }

            // Invia comando STARTTLS
            EMAIL_SENDER_DEBUG_PRINTLN(F("Sending STARTTLS command"));
            baseClient.println(F("STARTTLS"));
            response = awaitSMTPResponse(baseClient, "220", "STARTTLS not supported");
            if (!response.status) {
                EMAIL_SENDER_DEBUG_PRINTLN(F("STARTTLS command failed"));
                baseClient.flush();
                baseClient.stop();
                return response;
            }

            EMAIL_SENDER_DEBUG_PRINTLN(F("STARTTLS accepted, upgrading connection with SSLClient..."));

            // Usa il metodo startTLS di SSLClient per fare l'upgrade della connessione
            sslClient.startTLS(this->smtp_server, this->smtp_port);

            EMAIL_SENDER_DEBUG_PRINTLN(F("SSL upgrade completed"));

            // Attendi che la connessione SSL sia completamente stabilita
            delay(100);

            // Svuota eventuali dati residui nel buffer
            while (sslClient.available() > 0) {
                sslClient.read();
            }

            // Ora usa sslClient per il resto della comunicazione
            activeClient = &sslClient;
#else
            EMAIL_SENDER_DEBUG_PRINTLN(F("ERROR: STARTTLS requested but EMAIL_ENABLE_INTERNAL_SSLCLIENT not defined"));
            response.desc = F("STARTTLS not available - enable EMAIL_ENABLE_INTERNAL_SSLCLIENT");
            response.code = F("2");
            response.status = false;
            return response;
#endif
        } else {
            // Per altre porte (465 - SSL implicito), usa WiFiClientSecure
            localClient.setInsecure();
            bool connected = localClient.connect(this->smtp_server, this->smtp_port);
            if (!connected) {
                IPAddress ip;
                if (WiFi.hostByName(this->smtp_server, ip)) {
                    EMAIL_SENDER_DEBUG_PRINT(F("Resolved IP: "));
                    EMAIL_SENDER_DEBUG_PRINTLN(ip);
                    connected = localClient.connect(ip, this->smtp_port);
                }
            }

            if (!connected) {
                response.desc = F("Could not connect to mail server");
                response.code = F("2");
                response.status = false;
                localClient.flush();
                localClient.stop();
                return response;
            }

            response = awaitSMTPResponse(localClient, "220", "Connection Error");
            if (!response.status) {
                localClient.flush();
                localClient.stop();
                return response;
            }

            String commandHELO = this->useEHLO ? "EHLO" : "HELO";
            String helo = commandHELO + " "+String(publicIPDescriptor)+" ";
            EMAIL_SENDER_DEBUG_PRINTLN(helo);
            localClient.println(helo);

            if (this->useEHLO) {
                response = awaitSMTPResponseDrain(localClient, "250", "Identification error");
            } else {
                response = awaitSMTPResponse(localClient, "250", "Identification error");
            }
            if (!response.status) {
                localClient.flush();
                localClient.stop();
                return response;
            }

            activeClient = &localClient;
        }
#else
        bool connected = localClient.connect(this->smtp_server, this->smtp_port);
        if (!connected) {
            response.desc = F("Could not connect to mail server");
            response.code = F("2");
            response.status = false;
            localClient.flush();
            localClient.stop();
            return response;
        }

        response = awaitSMTPResponse(localClient, "220", "Connection Error");
        if (!response.status) {
            localClient.flush();
            localClient.stop();
            return response;
        }

        String commandHELO = this->useEHLO ? "EHLO" : "HELO";
        String helo = commandHELO + " "+String(publicIPDescriptor)+" ";
        EMAIL_SENDER_DEBUG_PRINTLN(helo);
        localClient.println(helo);

        if (this->useEHLO) {
            response = awaitSMTPResponseDrain(localClient, "250", "Identification error");
        } else {
            response = awaitSMTPResponse(localClient, "250", "Identification error");
        }
        if (!response.status) {
            localClient.flush();
            localClient.stop();
            return response;
        }

        activeClient = &localClient;
#endif

        if (activeClient == nullptr) {
            activeClient = &localClient;
        }
    }

    // Use a single reference for the selected client
    Client &client = *activeClient;

    // Legacy EHLO multi-response handling is skipped because we use the Drain variant above
    bool skipLegacyEHLO = true; // abbiamo usato Drain qui sopra
    if (!skipLegacyEHLO && this->useEHLO == true && this->additionalResponseLineOnHELO == 0) {
        if (DEFAULT_EHLO_RESPONSE_COUNT == 'a'){
            this->additionalResponseLineOnHELO = 255;
        } else {
            this->additionalResponseLineOnHELO = DEFAULT_EHLO_RESPONSE_COUNT;
        }
    }

    if (!skipLegacyEHLO && this->additionalResponseLineOnHELO > 0){
        for (int i = 0; i<=this->additionalResponseLineOnHELO; i++) {
            response = awaitSMTPResponse(*activeClient, "250", "EHLO error", 2500);
            if (!response.status && response.code == F("1")) {
                if (this->additionalResponseLineOnHELO == 255) break;
                else {
                    response.desc = F("Timeout! Reduce additional HELO response line count or set it to 255!");
                    activeClient->flush();
                    activeClient->stop();
                    return response;
                }
            }
        }
    }

    bool useAuth = this->useAuth;
    if (useAuth){
        if (this->isSASLLogin == true){

            int size = 1 + strlen(this->email_login)+ strlen(this->email_password)+2;
            char * logPass = (char *) malloc(size);

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

            String auth = "AUTH PLAIN "+String(encode64_f(logPass, size));
            EMAIL_SENDER_DEBUG_PRINTLN(auth);
            activeClient->println(auth);
            free(logPass);
        }
#if defined(ESP32)
        else if (this->isCramMD5Login == true) {
            EMAIL_SENDER_DEBUG_PRINTLN(F("AUTH CRAM-MD5"));
            activeClient->println(F("AUTH CRAM-MD5"));
            response = awaitSMTPResponse(*activeClient,"334","No digest error");
            if (!response.status) { activeClient->flush(); activeClient->stop(); return response; };
            _serverResponce = _serverResponce.substring(4);

            size_t b64l = _serverResponce.length()-1;
            const unsigned char * b64 = (const unsigned char *)_serverResponce.c_str();
            EMAIL_SENDER_DEBUG_PRINTLN("B64digest="+String((char *)b64) + " Len=" + String((int)b64l));

            unsigned char digest[256];
            size_t len;
            int e = mbedtls_base64_decode(digest, sizeof(digest), &len, b64, b64l);
            if (e || len < 3 || len >= 256) {
                response.code = F("999"); response.desc = F("Invalid digest"); activeClient->flush(); activeClient->stop(); return response;
            };

            mbedtls_md_context_t ctx;
            mbedtls_md_type_t md_type = MBEDTLS_MD_MD5;
            unsigned char md5[16];

            mbedtls_md_init(&ctx);
            mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
            mbedtls_md_hmac_starts(&ctx, (const unsigned char *)this->email_password, strlen(this->email_password));
            mbedtls_md_hmac_update(&ctx, digest, len);
            mbedtls_md_hmac_finish(&ctx, md5);
            mbedtls_md_free(&ctx);

            String rsp = String(this->email_login) + " ";
            for(int i = 0; i < (int)sizeof(md5); i++) {
                unsigned char c = md5[i];
                char h[16+1] = "0123456789abcdef";
                rsp += String(h[ (c >> 4) &0xF]) +  String(h[ (c >> 0) &0xF]);
            };
            EMAIL_SENDER_DEBUG_PRINTLN(encode64((char*)rsp.c_str()));
            activeClient->println(encode64((char*)rsp.c_str()));
        }
#endif
        else{
            EMAIL_SENDER_DEBUG_PRINTLN(F("AUTH LOGIN:"));
            activeClient->println(F("AUTH LOGIN"));
            // Attendi richiesta username
            response = awaitSMTPResponse(*activeClient, "334", "AUTH LOGIN: no username challenge");
            if (!response.status) { activeClient->flush(); activeClient->stop(); return response; }

            // Invia username in base64 e attendi richiesta password
            EMAIL_SENDER_DEBUG_PRINTLN(encode64(this->email_login));
            activeClient->println(encode64(this->email_login));
            response = awaitSMTPResponse(*activeClient, "334", "AUTH LOGIN: no password challenge");
            if (!response.status) { activeClient->flush(); activeClient->stop(); return response; }

            // Invia password in base64, la conferma arriva con 235
            EMAIL_SENDER_DEBUG_PRINTLN(encode64(this->email_password));
            activeClient->println(encode64(this->email_password));
        }
        response = awaitSMTPResponse(*activeClient, "235", "SMTP AUTH error");
        if (!response.status) { activeClient->flush(); activeClient->stop(); return response; }
    }

    EMAIL_SENDER_DEBUG_PRINT(F("MAIL FROM: <"));
    EMAIL_SENDER_DEBUG_PRINT(this->email_from);
    EMAIL_SENDER_DEBUG_PRINTLN(F(">"));

    activeClient->print(F("MAIL FROM: <"));
    activeClient->print(this->email_from);
    activeClient->println(F(">"));
    awaitSMTPResponse(*activeClient);

    int cont;
    for (cont=0;cont<(sizeOfTo+sizeOfCc+sizeOfCCn);cont++){
        EMAIL_SENDER_DEBUG_PRINT(F("RCPT TO: <"));
        EMAIL_SENDER_DEBUG_PRINT(to[cont]);
        EMAIL_SENDER_DEBUG_PRINTLN(F(">"));
        activeClient->print(F("RCPT TO: <"));
        activeClient->print(to[cont]);
        activeClient->println(F(">"));
        awaitSMTPResponse(*activeClient);
    }

    EMAIL_SENDER_DEBUG_PRINTLN(F("DATA:"));
    activeClient->println(F("DATA"));

    response = awaitSMTPResponse(*activeClient, "354", "SMTP DATA error");
    if (!response.status) { activeClient->flush(); activeClient->stop(); return response; }

    activeClient->print(F("From: "));
    if (this->name_from){
        activeClient->print(F("\""));  // ✅ Aggiunge virgolette
        activeClient->print(this->name_from);
        activeClient->print(F("\" "));  // ✅ Chiude virgolette
    }
    activeClient->print(F("<"));
    activeClient->print(this->email_from);
    activeClient->println(F(">"));

    activeClient->print(F("To: "));
    for (cont=0;cont<sizeOfTo;cont++){
        activeClient->print(F("<"));
        activeClient->print(to[cont]);
        activeClient->print(">");
        if (cont!=sizeOfTo-1){ activeClient->print(","); }
    }
    activeClient->println();

    if (sizeOfCc>0){
        activeClient->print(F("Cc: "));
        for (;cont<sizeOfTo+sizeOfCc;cont++){
            activeClient->print(F("<"));
            activeClient->print(to[cont]);
            activeClient->print(">");
            if (cont!=sizeOfTo+sizeOfCc-1){ activeClient->print(","); }
        }
        activeClient->println();
    }

    if (sizeOfCCn>0){
        activeClient->print(F("Bcc: "));  // ✅ CORRETTO: era "CCn"
        for (;cont<sizeOfTo+sizeOfCc+sizeOfCCn;cont++){
            activeClient->print(F("<"));
            activeClient->print(to[cont]);
            activeClient->print(">");
            if (cont!=sizeOfTo+sizeOfCc+sizeOfCCn-1){ activeClient->print(","); }
        }
        activeClient->println();
    }

    activeClient->print(F("Subject: "));
    activeClient->println(email.subject);

#ifdef MANAGE_DATE_HEADER
    #if defined(ESP32) || defined(ESP8266)
        configTime(0, 0, "pool.ntp.org");
        struct tm timeinfo;
        char buf[64];
        if (getLocalTime(&timeinfo, 5000)) {  // wait up to 5s
            strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S +0000", &timeinfo);
        } else {
            // Fallback to a fixed RFC2822-like date to ensure exactly one Date header exists
            strcpy(buf, "Mon, 01 Jan 2001 00:00:00 +0000");
        }
        activeClient->print(F("Date: "));
        activeClient->println(buf);
    #else
        // Generic fallback for other platforms
        activeClient->println(F("Date: Mon, 01 Jan 2001 00:00:00 +0000"));
    #endif
#endif

    activeClient->println(F("MIME-Version: 1.0"));
    activeClient->println(F("Content-Type: Multipart/mixed; boundary=frontier"));

    activeClient->println();  // ✅ RIGA VUOTA OBBLIGATORIA (RFC 2822)

    // ✅ Flush per assicurare che gli header siano inviati
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || defined(__AVR_ATmega2560__)
        activeClient->flush();
        delay(100);  // Piccolo delay per Arduino Mega/Ethernet
    #endif

    activeClient->println(F("--frontier"));

    activeClient->print(F("Content-Type: "));
    activeClient->print(email.mime);
    activeClient->println(F("; charset=utf-8"));
    activeClient->println(F("Content-Transfer-Encoding: 7bit"));
    activeClient->println();
    if (email.mime==F("text/html")){
        activeClient->print(F("<!DOCTYPE html><html lang=\"en\">"));
        activeClient->print(email.message);
        activeClient->println(F("</html>"));
    }else{
        activeClient->println(email.message);
    }
    activeClient->println();

    // ✅ Flush dopo l'invio del corpo del messaggio
    #if defined(ARDUINO_AVR_MEGA2560) || defined(ARDUINO_AVR_MEGA) || defined(__AVR_ATmega2560__)
        activeClient->flush();
        delay(100);
    #endif

#ifdef STORAGE_INTERNAL_ENABLED
  bool spiffsActive = false;
#endif
#ifdef STORAGE_EXTERNAL_ENABLED
  bool sdActive = false;
#endif

#if defined(ENABLE_ATTACHMENTS) && (defined(STORAGE_EXTERNAL_ENABLED) || defined(STORAGE_INTERNAL_ENABLED))
//  if ((sizeof(attachs) / sizeof(attachs[0]))>0){
  if (sizeof(attachments)>0 && attachments.number>0){

	  EMAIL_SENDER_DEBUG_PRINT(F("Array: "));
//	  for (int i = 0; i<(sizeof(attachs) / sizeof(attachs[0])); i++){
	  for (int i = 0; i<attachments.number; i++){
		  uint8_t tBuf[64];

		  // Per gli Stream attachment, l'url non è necessario
		  if (attachments.fileDescriptor[i].storageType != EMAIL_STORAGE_TYPE_STREAM &&
		      attachments.fileDescriptor[i].url.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no filename specified for the file "+attachments.fileDescriptor[i].filename;
			  response.status = false;
			  client.flush();
			  client.stop();

			  return response;
		  }
		  if (attachments.fileDescriptor[i].mime.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no mime type specified for the file "+attachments.fileDescriptor[i].url;
			  response.status = false;
			  client.flush();
			  client.stop();

			  return response;
		  }
		  if (attachments.fileDescriptor[i].filename.length()==0){
			  EMailSender::Response response;
			  response.code = F("400");
			  response.desc = "Error no filename specified for the file "+attachments.fileDescriptor[i].url;
			  response.status = false;
			  client.flush();
			  client.stop();

			  return response;
		  }

		  EMAIL_SENDER_DEBUG_PRINTLN(attachments.fileDescriptor[i].filename);
		  EMAIL_SENDER_DEBUG_PRINTLN(F("--frontier"));
		  client.println(F("--frontier"));
		  EMAIL_SENDER_DEBUG_PRINTLN(F("Content-Type: "));
		  client.print(F("Content-Type: "));
		  EMAIL_SENDER_DEBUG_PRINTLN(attachments.fileDescriptor[i].mime);
		  client.print(attachments.fileDescriptor[i].mime);
		  EMAIL_SENDER_DEBUG_PRINTLN(F("; charset=utf-8"));
		  client.println(F("; charset=utf-8"));

		  if (attachments.fileDescriptor[i].encode64){
			  client.println(F("Content-Transfer-Encoding: base64"));
		  }

		  client.print(F("Content-Disposition: attachment; filename="));
		  client.print(attachments.fileDescriptor[i].filename);
		  client.println(F("\n"));
		  EMAIL_SENDER_DEBUG_PRINT(F("Readed filename: "));
		  EMAIL_SENDER_DEBUG_PRINTLN(attachments.fileDescriptor[i].filename);

//		  EMAIL_SENDER_DEBUG_PRINT(F("Check if exist: "));
//		  EMAIL_SENDER_DEBUG_PRINTLN(INTERNAL_STORAGE_CLASS.exists(attachments.fileDescriptor[i].url.c_str()));

		  int clientCount = 0;

#ifdef STORAGE_INTERNAL_ENABLED
			if (attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SPIFFS ||
				attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_LITTLE_FS ||
				attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SPIFM ||
				attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_FFAT){
	#ifdef OPEN_CLOSE_INTERNAL
				if (!INTERNAL_STORAGE_CLASS.exists(attachments.fileDescriptor[i].url.c_str())){
				    EMAIL_SENDER_DEBUG_PRINTLN(F("Begin internal storage!"));

					#if (INTERNAL_STORAGE == STORAGE_SPIFM)
					Adafruit_FlashTransport_SPI flashTransport(SPIFM_CS_PIN, SPI); // Set CS and SPI interface
					Adafruit_SPIFlash flash(&flashTransport);

						  // Initialize flash library and check its chip ID.
					if (!flash.begin()) {
						EMailSender::Response response;
						  response.code = F("500");
						  response.desc = F("Error, failed to initialize flash chip!");
						  response.status = false;
						  client.flush();
						  client.stop();

						  return response;
					} // close flash.begin()

					if(!(INTERNAL_STORAGE_CLASS.begin(&flash))){
					#else
					if(!(INTERNAL_STORAGE_CLASS.begin())){
					#endif
						  EMailSender::Response response;
						  response.code = F("500");
						  response.desc = F("Error on startup filesystem!");
						  response.status = false;
						  client.flush();
						  client.stop();

						  return response;
					} // Close INTERNAL_STORAGE_CLASS.begin

					spiffsActive = true;
					EMAIL_SENDER_DEBUG_PRINTLN("SPIFFS BEGIN, ACTIVE");
				} // Close INTERNAL_STORAGE_CLASS.exists

	#endif

				EMAIL_SENDER_DEBUG_PRINT(F("Try to open "));
				EMAIL_SENDER_DEBUG_PRINTLN(attachments.fileDescriptor[i].url);
				EMAIL_FILE myFile = INTERNAL_STORAGE_CLASS.open(attachments.fileDescriptor[i].url, EMAIL_FILE_READ);
				  if(myFile) {
					  EMAIL_SENDER_DEBUG_PRINT(F("Filename -> "));
					  EMAIL_SENDER_DEBUG_PRINTLN(myFile.name());
					  if (attachments.fileDescriptor[i].encode64){
						  encode(&myFile, &client);
					  }
					  else{
						while(myFile.available()) {
						clientCount = myFile.read(tBuf,64);
						EMAIL_SENDER_DEBUG_PRINTLN(clientCount);
						  client.write((byte*)tBuf,clientCount);
						}
					  }
					myFile.close();

					client.println();
				  } // Else myfile
				  else {
					  EMailSender::Response response;
					  response.code = F("404");
					  response.desc = "Error opening attachments file "+attachments.fileDescriptor[i].url;
					  response.status = false;
					  client.flush();
					  client.stop();

					  return response;
				  } // Close myfile
				} // Close storageType

#endif
#ifdef STORAGE_EXTERNAL_ENABLED
			if (attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SD){
#ifdef OPEN_CLOSE_SD
				 EMAIL_SENDER_DEBUG_PRINTLN(F("SD Check"));
				 if (!EXTERNAL_STORAGE_CLASS.exists(attachments.fileDescriptor[i].url.c_str())){
#if EXTERNAL_STORAGE == STORAGE_SD || EXTERNAL_STORAGE == STORAGE_SDFAT2 || EXTERNAL_STORAGE == STORAGE_SDFAT_RP2040_ESP8266
					if(!EXTERNAL_STORAGE_CLASS.begin(SD_CS_PIN)){
						  response.code = F("500");
						  response.desc = F("Error on startup SD filesystem!");
						  response.status = false;
						  client.flush();
						  client.stop();

						  return response;
					} // Close EXTERNAL_STORAGE_CLASS.begin
#elif EXTERNAL_STORAGE == STORAGE_SPIFM
					Adafruit_FlashTransport_SPI flashTransport(SS, SPI); // Set CS and SPI interface
					Adafruit_SPIFlash flash(&flashTransport);

				  if (!EXTERNAL_STORAGE_CLASS.begin(&flash)) {
					  response.code = F("500");
					  response.desc = F("Error on startup SDFAT2 filesystem!");
					  client.flush();
					  client.stop();

					  return response;
				  }
#endif
					sdActive = true;
				 } // Close EXTERNAL_STORAGE_CLASS.exists
#endif

			    EMAIL_SENDER_DEBUG_PRINTLN(F("Open file: "));
			EMAIL_FILE_EX myFile = EXTERNAL_STORAGE_CLASS.open(attachments.fileDescriptor[i].url.c_str());

			  if(myFile) {
				  myFile.seek(0);
				  EMAIL_SENDER_DEBUG_PRINTLN(F("OK"));
				  if (attachments.fileDescriptor[i].encode64){
					  EMAIL_SENDER_DEBUG_PRINTLN(F("BASE 64"));
					  encode(&myFile, &client);
				  }else{
					  EMAIL_SENDER_DEBUG_PRINTLN(F("NORMAL"));
					while(myFile.available()) {
						clientCount = myFile.read(tBuf,64);
						client.write((byte*)tBuf,clientCount);
					}
				  }
				myFile.close();

				client.println();
			  } // Else myfile
			  else {
				  response.code = F("404");
				  response.desc = "Error opening attachments file "+attachments.fileDescriptor[i].url;
				  response.status = false;
				  client.flush();
				  client.stop();

				  return response;
			  } // Close myFile

			} // Close storageType==EMAIL_STORAGE_TYPE_SD
#else
	if (attachments.fileDescriptor[i].storageType==EMAIL_STORAGE_TYPE_SD){
		response.code = F("500");
		response.desc = F("EMAIL_STORAGE_TYPE_SD not enabled on EMailSenderKey.h");
		response.status = false;
		  client.flush();
		  client.stop();

		return response;
	}
#endif

// NEW: Handle Stream attachments
		if (attachments.fileDescriptor[i].storageType == EMAIL_STORAGE_TYPE_STREAM) {
			EMAIL_SENDER_DEBUG_PRINTLN(F("Stream attachment"));

			if (attachments.fileDescriptor[i].stream == nullptr) {
				response.code = F("500");
				response.desc = F("Stream pointer is null for EMAIL_STORAGE_TYPE_STREAM");
				response.status = false;
				client.flush();
				client.stop();
				return response;
			}

			if (attachments.fileDescriptor[i].streamSize == 0) {
				response.code = F("500");
				response.desc = F("Stream size must be specified for EMAIL_STORAGE_TYPE_STREAM");
				response.status = false;
				client.flush();
				client.stop();
				return response;
			}

			Stream* streamPtr = attachments.fileDescriptor[i].stream;
			size_t totalSize = attachments.fileDescriptor[i].streamSize;
			size_t bytesRead = 0;

			EMAIL_SENDER_DEBUG_PRINT(F("Stream size: "));
			EMAIL_SENDER_DEBUG_PRINTLN(totalSize);

			if (attachments.fileDescriptor[i].encode64) {
				EMAIL_SENDER_DEBUG_PRINTLN(F("BASE 64 encoding from Stream"));
				encode(streamPtr, &client);
			} else {
				EMAIL_SENDER_DEBUG_PRINTLN(F("NORMAL read from Stream"));
				while (bytesRead < totalSize && streamPtr->available()) {
					size_t toRead = min((size_t)64, totalSize - bytesRead);
					clientCount = streamPtr->readBytes(tBuf, toRead);
					if (clientCount > 0) {
						client.write((byte*)tBuf, clientCount);
						bytesRead += clientCount;
					} else {
						break;
					}
				}

				if (bytesRead < totalSize) {
					EMAIL_SENDER_DEBUG_PRINT(F("Warning: Read "));
					EMAIL_SENDER_DEBUG_PRINT(bytesRead);
					EMAIL_SENDER_DEBUG_PRINT(F(" bytes, expected "));
					EMAIL_SENDER_DEBUG_PRINTLN(totalSize);
				}
			}

			client.println();
			EMAIL_SENDER_DEBUG_PRINTLN(F("Stream attachment sent"));
		} // Close storageType==EMAIL_STORAGE_TYPE_STREAM

// NEW: Handle String attachments (direct)
		if (attachments.fileDescriptor[i].storageType == EMAIL_STORAGE_TYPE_STRING) {
			EMAIL_SENDER_DEBUG_PRINTLN(F("String attachment"));

			if (attachments.fileDescriptor[i].content.length() == 0) {
				response.code = F("500");
				response.desc = F("Content string is empty for EMAIL_STORAGE_TYPE_STRING");
				response.status = false;
				client.flush();
				client.stop();
				return response;
			}

			String& stringContent = attachments.fileDescriptor[i].content;
			size_t totalSize = stringContent.length();

			EMAIL_SENDER_DEBUG_PRINT(F("String size: "));
			EMAIL_SENDER_DEBUG_PRINTLN(totalSize);

			if (attachments.fileDescriptor[i].encode64) {
				EMAIL_SENDER_DEBUG_PRINTLN(F("BASE 64 encoding from String"));
				// For base64 encoding, we need to send byte by byte
				for (size_t pos = 0; pos < totalSize; pos += 64) {
					size_t chunkSize = min((size_t)64, totalSize - pos);
					for (size_t j = 0; j < chunkSize; j++) {
						tBuf[j] = stringContent.charAt(pos + j);
					}
					// Encode and send chunk
					const char* encoded = encode64_f((char*)tBuf, chunkSize);
					client.print(encoded);
				}
			} else {
				EMAIL_SENDER_DEBUG_PRINTLN(F("NORMAL write from String"));
				// Send string content directly in chunks
				for (size_t pos = 0; pos < totalSize; pos += 512) {
					size_t chunkSize = min((size_t)512, totalSize - pos);
					String chunk = stringContent.substring(pos, pos + chunkSize);
					client.print(chunk);
				}
			}

			client.println();
			EMAIL_SENDER_DEBUG_PRINTLN(F("String attachment sent"));
		} // Close storageType==EMAIL_STORAGE_TYPE_STRING
	  } // Close attachment cycle
	  client.println();
	  client.println(F("--frontier--"));
#ifdef STORAGE_EXTERNAL_ENABLED
	  #ifdef OPEN_CLOSE_SD
		  if (sdActive){
			  EMAIL_SENDER_DEBUG_PRINTLN(F("SD end"));
		#ifndef ARDUINO_ESP8266_RELEASE_2_4_2
			#if EXTERNAL_STORAGE != STORAGE_SDFAT_RP2040_ESP8266
			  EXTERNAL_STORAGE_CLASS.end();
			#endif
		#endif
			  EMAIL_SENDER_DEBUG_PRINTLN(F("SD end 2"));
		  }
	  #endif
#endif

#ifdef STORAGE_INTERNAL_ENABLED
	#ifdef OPEN_CLOSE_INTERNAL
		#if INTERNAL_STORAGE != STORAGE_SPIFM
		  if (spiffsActive){
			  INTERNAL_STORAGE_CLASS.end();
			  EMAIL_SENDER_DEBUG_PRINTLN(F("SPIFFS END"));
		  }
		#endif
	#endif
#endif

  } // Close attachement enable

#endif
  EMAIL_SENDER_DEBUG_PRINTLN(F("Message end"));
  client.println(F("."));

  response = awaitSMTPResponse(client, "250", "Sending message error");
  if (!response.status) {
	  client.flush();
	  client.stop();
	  return response;
  }

  client.println(F("QUIT"));

  response = awaitSMTPResponse(client, "221", "SMTP QUIT error");
  if (!response.status) {
	  client.flush();
	  client.stop();
	  return response;
  }

  response.status = true;
  response.code = F("0");
  response.desc = F("Message sent!");

  client.flush();
  client.stop();

  return response;
}
