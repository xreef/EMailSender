#pragma once
#include <Arduino.h>
#include <Client.h>

// Se è abilitato l'uso interno (EMAIL_ENABLE_OPENSLAB_SSLCLIENT) e NON è stato richiesto l'esterno,
// includi la copia interna del client OSU.
#if defined(EMAIL_ENABLE_OPENSLAB_SSLCLIENT) && !defined(EMAIL_ENABLE_EXTERNAL_SSLCLIENT_OPENSLAB)
  #include "sslclientosu/third_party/SSLClient.h"
#else
  // Uso esterno: richiede la libreria OPEnSLab-OSU/SSLClient installata
  #ifdef __has_include
    #if __has_include(<SSLClient.h>)
      #include <SSLClient.h>
    #else
      #error "SSLClient.h non trovato: installa la libreria OPEnSLab-OSU/SSLClient oppure abilita EMAIL_ENABLE_OPENSLAB_SSLCLIENT per usare quella interna."
    #endif
  #else
    #include <SSLClient.h>
  #endif
#endif

namespace sslclientosu {

// Thin adapter around OPEnSLab-OSU SSLClient (BearSSL) implementing Arduino Client
class OsuSSLClient : public Client {
 public:
  OsuSSLClient(Client &baseClient,
               const br_x509_trust_anchor *trustAnchors,
               size_t trustAnchorsCount,
               int analogPin = -1)
      : _base(&baseClient),
        _ssl(baseClient, trustAnchors, trustAnchorsCount, analogPin) {}

  // No-op: OPEnSLab SSLClient non espone setInsecure; mantenuto per compatibilità API
  void setInsecure() {}

  // Client interface
  int connect(const char *host, uint16_t port) override { return _ssl.connect(host, port); }
  int connect(IPAddress ip, uint16_t port) override { return _ssl.connect(ip, port); }
  void stop() override { _ssl.stop(); }
  uint8_t connected() override { return _ssl.connected(); }
  operator bool() override { return static_cast<bool>(_ssl); }

  // NOTE: the following functions are not implemented and will return 0
  // or whatever is appropriate.
  size_t write(uint8_t) override;
  size_t write(const uint8_t *buf, size_t size) override;
  int available() override;
  int read() override;
  int read(uint8_t *buf, size_t size) override;
  int peek() override;
  void flush() override;

 private:
  Client *_base;
  ::SSLClient _ssl;
};

} // namespace sslclientosu

#ifndef OSUSSLCLIENTADAPTER_H_
#define OSUSSLCLIENTADAPTER_H_

#if defined(EMAIL_ENABLE_OPENSLAB_SSLCLIENT)

// Forward declarations to break circular dependency
class EMailClient;
namespace EMailSender {
    enum DebugLevel;
}

#include "sslclientosu/third_party/SSLClient.h"

class OSUSSLClientAdapter: public EMailClient {
public:
	OSUSSLClientAdapter(Client& client, const br_x509_trust_anchor* trust_anchors, const int trust_anchors_num, const int analog_pin = -1, const int max_session = 1);
	virtual ~OSUSSLClientAdapter();

	uint8_t connected();
	int connect(IPAddress ip, uint16_t port);
	int connect(const char *host, uint16_t port);
	size_t write(const uint8_t *buf, size_t size);
	size_t write(uint8_t);
	int available();
	int read();
	int read(uint8_t *buf, size_t size);
	int peek();
	void flush();
	void stop();
	void setDebug(EMailSender::DebugLevel level);

private:
	SSLClient* sslClient;
};

#endif

#endif /* OSUSSLCLIENTADAPTER_H_ */
