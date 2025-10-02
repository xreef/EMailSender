// SSLClient.h aggiornato dalla libreria ESP_SSLClient
// https://github.com/mobizt/ESP_SSLClient
#pragma once
#include <Arduino.h>
#include <Client.h>
#include "CertStore.h"
#include "Utility.h"

#if defined(ESP32)
#include <mbedtls/ssl.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#endif

namespace sslclient {

class SSLClient : public Client {
public:
    SSLClient(Client &client, CertStoreBase *certStore = nullptr);
    ~SSLClient();
    int connect(const char *host, uint16_t port) override;
    int connect(IPAddress ip, uint16_t port) override;
    void stop() override;
    size_t write(uint8_t) override;
    size_t write(const uint8_t *buf, size_t size) override;
    int available() override;
    int read() override;
    int read(uint8_t *buf, size_t size) override;
    int peek() override;
    void flush() override;
    uint8_t connected() override;
    operator bool() override;

    void setInsecure();
    void setTrustAnchors(CertStoreBase *certStore);
    void startTLS(const char *host, uint16_t port);

private:
    Client *_baseClient;
    CertStoreBase *_certStore;

#if defined(ESP32)
    bool _usingTLS = false;
    bool _insecure = true;
    mbedtls_ssl_context _ssl;
    mbedtls_ssl_config _conf;
    mbedtls_ctr_drbg_context _ctr_drbg;
    mbedtls_entropy_context _entropy;
    String _sniHost;

    static int bio_send(void *ctx, const unsigned char *buf, size_t len);
    static int bio_recv(void *ctx, unsigned char *buf, size_t len);
#endif
};

} // namespace sslclient
