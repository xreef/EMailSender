// SSLClient.cpp adapted from the ESP_SSLClient library
// https://github.com/mobizt/ESP_SSLClient

// Questo file contiene l'implementazione SSLClient per ESP32/ESP8266
// Deve essere compilato SOLO se EMAIL_ENABLE_INTERNAL_SSLCLIENT è abilitato
// Ultima modifica: forza rebuild per applicare le guard
#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)

#include "SSLClient.h"

namespace sslclient {

SSLClient::SSLClient(Client &client, CertStoreBase *certStore) : _baseClient(&client), _certStore(certStore) {}
SSLClient::~SSLClient() { stop(); }
int SSLClient::connect(const char *host, uint16_t port) { return _baseClient->connect(host, port); }
int SSLClient::connect(IPAddress ip, uint16_t port) { return _baseClient->connect(ip, port); }
void SSLClient::stop() {
#if defined(ESP32)
    if (_usingTLS) {
        mbedtls_ssl_free(&_ssl);
        mbedtls_ssl_config_free(&_conf);
        mbedtls_ctr_drbg_free(&_ctr_drbg);
        mbedtls_entropy_free(&_entropy);
        _usingTLS = false;
        _sniHost = String();
    }
#endif
    _baseClient->stop();
}
size_t SSLClient::write(uint8_t b) { return write(&b, 1); }
size_t SSLClient::write(const uint8_t *buf, size_t size) {
#if defined(ESP32)
    if (_usingTLS) {
        int ret = mbedtls_ssl_write(&_ssl, buf, size);
        if (ret >= 0) return (size_t)ret;
        return 0;
    }
#endif
    return _baseClient->write(buf, size);
}
int SSLClient::available() {
#if defined(ESP32)
    if (_usingTLS) {
        // We cannot know decrypted bytes available; approximate with transport bytes
        return _baseClient->available();
    }
#endif
    return _baseClient->available();
}
int SSLClient::read() {
    uint8_t b;
    int n = read(&b, 1);
    return (n == 1) ? b : -1;
}
int SSLClient::read(uint8_t *buf, size_t size) {
#if defined(ESP32)
    if (_usingTLS) {
        int ret = mbedtls_ssl_read(&_ssl, buf, size);
        if (ret > 0) return ret;
        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) return 0;
        return -1;
    }
#endif
    return _baseClient->read(buf, size);
}
int SSLClient::peek() {
#if defined(ESP32)
    if (_usingTLS) {
        // Not implemented with mbedTLS without internal buffer: return -1
        return -1;
    }
#endif
    return _baseClient->peek();
}
void SSLClient::flush() { _baseClient->flush(); }
uint8_t SSLClient::connected() { return _baseClient->connected(); }
SSLClient::operator bool() { return *_baseClient ? true : false; }
void SSLClient::setInsecure() {
#if defined(ESP32)
    _insecure = true;
#endif
}
void SSLClient::setTrustAnchors(CertStoreBase *certStore) { _certStore = certStore; }

#if defined(ESP32)
int SSLClient::bio_send(void *ctx, const unsigned char *buf, size_t len) {
    SSLClient *self = static_cast<SSLClient *>(ctx);
    if (!self || !self->_baseClient) return MBEDTLS_ERR_SSL_INTERNAL_ERROR;
    size_t written = self->_baseClient->write(buf, len);
    if (written > 0) return (int)written;
    if (self->_baseClient->connected()) return MBEDTLS_ERR_SSL_WANT_WRITE;
    return MBEDTLS_ERR_SSL_INTERNAL_ERROR;
}
int SSLClient::bio_recv(void *ctx, unsigned char *buf, size_t len) {
    SSLClient *self = static_cast<SSLClient *>(ctx);
    if (!self || !self->_baseClient) return MBEDTLS_ERR_SSL_INTERNAL_ERROR;
    int avail = self->_baseClient->available();
    if (avail <= 0) return MBEDTLS_ERR_SSL_WANT_READ;
    int toRead = (int)len;
    if (avail < toRead) toRead = avail;
    int r = self->_baseClient->read(buf, toRead);
    if (r > 0) return r;
    if (self->_baseClient->connected()) return MBEDTLS_ERR_SSL_WANT_READ;
    return MBEDTLS_ERR_SSL_INTERNAL_ERROR;
}
#endif

void SSLClient::startTLS(const char *host, uint16_t /*port*/) {
#if defined(ESP32)
    // Initialize mbedTLS and perform handshake on the already-connected socket
    _sniHost = host ? String(host) : String();

    mbedtls_ssl_init(&_ssl);
    mbedtls_ssl_config_init(&_conf);
    mbedtls_ctr_drbg_init(&_ctr_drbg);
    mbedtls_entropy_init(&_entropy);

    const char *pers = "sslclient_starttls";
    int ret = mbedtls_ctr_drbg_seed(&_ctr_drbg, mbedtls_entropy_func, &_entropy, (const unsigned char *)pers, strlen(pers));
    if (ret != 0) { stop(); return; }

    ret = mbedtls_ssl_config_defaults(&_conf, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
    if (ret != 0) { stop(); return; }

    if (_insecure) mbedtls_ssl_conf_authmode(&_conf, MBEDTLS_SSL_VERIFY_NONE);
    else mbedtls_ssl_conf_authmode(&_conf, MBEDTLS_SSL_VERIFY_OPTIONAL);

    mbedtls_ssl_conf_rng(&_conf, mbedtls_ctr_drbg_random, &_ctr_drbg);

    ret = mbedtls_ssl_setup(&_ssl, &_conf);
    if (ret != 0) { stop(); return; }

    if (host) mbedtls_ssl_set_hostname(&_ssl, host);

    mbedtls_ssl_set_bio(&_ssl, this, (mbedtls_ssl_send_t *)SSLClient::bio_send, (mbedtls_ssl_recv_t *)SSLClient::bio_recv, nullptr);

    // Handshake loop
    while ((ret = mbedtls_ssl_handshake(&_ssl)) != 0) {
        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE) {
            delay(10);
            continue;
        }
        // failure
        stop();
        return;
    }

    _usingTLS = true;
#else
    (void)host;
#endif
}
// ...other advanced methods...

} // namespace sslclient

#endif // EMAIL_ENABLE_INTERNAL_SSLCLIENT
