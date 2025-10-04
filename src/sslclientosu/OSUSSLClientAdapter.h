#ifndef OSUSSLCLIENTADAPTER_H_
#define OSUSSLCLIENTADAPTER_H_

#include <Arduino.h>
#include <Client.h>
#include "third_party/SSLClient.h"
#include "third_party/bearssl.h"

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

  size_t write(uint8_t b) override { return _ssl.write(b); }
  size_t write(const uint8_t *buf, size_t size) override { return _ssl.write(buf, size); }
  int available() override { return _ssl.available(); }
  int read() override { return _ssl.read(); }
  int read(uint8_t *buf, size_t size) override { return _ssl.read(buf, size); }
  int peek() override { return _ssl.peek(); }
  void flush() override { _ssl.flush(); }

 private:
  Client *_base;
  SSLClient _ssl;
};

} // namespace sslclientosu

// Mantieni anche la vecchia interfaccia per compatibilità
class OSUSSLClientAdapter: public Client {
public:
	OSUSSLClientAdapter(Client& client, const br_x509_trust_anchor* trust_anchors, const int trust_anchors_num, const int analog_pin = -1, const int max_session = 1);
	virtual ~OSUSSLClientAdapter();

	uint8_t connected() override;
	int connect(IPAddress ip, uint16_t port) override;
	int connect(const char *host, uint16_t port) override;
	size_t write(const uint8_t *buf, size_t size) override;
	size_t write(uint8_t) override;
	int available() override;
	int read() override;
	int read(uint8_t *buf, size_t size) override;
	int peek() override;
	void flush() override;
	void stop() override;
	void setDebug(int level);

private:
	SSLClient* sslClient;
};

#endif /* OSUSSLCLIENTADAPTER_H_ */
