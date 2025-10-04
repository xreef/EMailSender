#include "OSUSSLClientAdapter.h"

namespace sslclientosu {
// Adapter completamente inline nell'header; nessuna implementazione qui.
} // namespace sslclientosu

// Implementazione della classe OSUSSLClientAdapter
OSUSSLClientAdapter::OSUSSLClientAdapter(Client& client, const br_x509_trust_anchor* trust_anchors,
                                        const int trust_anchors_num, const int analog_pin, const int max_session) {
    // Inizializza SSLClient con il livello di debug appropriato in base al parametro analog_pin
    // Più alto è analog_pin, più alto è il livello di debug (convenzione)
    SSLClient::DebugLevel debugLevel = SSLClient::SSL_WARN;  // default
    this->sslClient = new SSLClient(client, trust_anchors, trust_anchors_num, analog_pin, max_session, debugLevel);
}

OSUSSLClientAdapter::~OSUSSLClientAdapter() {
    // Chiama prima stop per garantire la chiusura sicura
    if (this->sslClient) {
        this->sslClient->stop();
        delete this->sslClient;
        this->sslClient = nullptr;
    }
}

uint8_t OSUSSLClientAdapter::connected() {
    return this->sslClient->connected();
}

int OSUSSLClientAdapter::connect(IPAddress ip, uint16_t port) {
    return this->sslClient->connect(ip, port);
}

int OSUSSLClientAdapter::connect(const char *host, uint16_t port) {
    return this->sslClient->connect(host, port);
}

size_t OSUSSLClientAdapter::write(const uint8_t *buf, size_t size) {
    return this->sslClient->write(buf, size);
}

size_t OSUSSLClientAdapter::write(uint8_t b) {
    return this->sslClient->write(b);
}

int OSUSSLClientAdapter::available() {
    return this->sslClient->available();
}

int OSUSSLClientAdapter::read() {
    return this->sslClient->read();
}

int OSUSSLClientAdapter::read(uint8_t *buf, size_t size) {
    return this->sslClient->read(buf, size);
}

int OSUSSLClientAdapter::peek() {
    return this->sslClient->peek();
}

void OSUSSLClientAdapter::flush() {
    this->sslClient->flush();
}

void OSUSSLClientAdapter::stop() {
    this->sslClient->stop();
}

// SSLClient non supporta la modifica del livello di debug dopo la creazione
// quindi questo metodo non fa nulla
void OSUSSLClientAdapter::setDebug(int level) {
    // Non fa nulla, il livello di debug può essere impostato solo nel costruttore
}
