// CertStore.h aggiornato dalla libreria ESP_SSLClient
// https://github.com/mobizt/ESP_SSLClient
#pragma once
#include <Arduino.h>

namespace sslclient {

class CertStoreBase {
public:
    virtual ~CertStoreBase() {}
    virtual void addCert(const char *cert) = 0;
    // ...altri metodi...
};

class CertStore : public CertStoreBase {
public:
    CertStore();
    ~CertStore();
    void addCert(const char *cert) override;
    // ...altri metodi...
private:
    // ...membri privati...
};

} // namespace sslclient

