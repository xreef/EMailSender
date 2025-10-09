// CertStore.cpp aggiornato dalla libreria ESP_SSLClient
// https://github.com/mobizt/ESP_SSLClient

// Questo file deve essere compilato SOLO se EMAIL_ENABLE_INTERNAL_SSLCLIENT è abilitato
// Ultima modifica: forza rebuild per applicare le guard

// Include EMailSenderKey.h per le definizioni delle macro
#include "../EMailSenderKey.h"

#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)

#include "CertStore.h"

namespace sslclient {

CertStore::CertStore() {}
CertStore::~CertStore() {}
void CertStore::addCert(const char *cert) {/* implementazione */}
// ...altri metodi...

} // namespace sslclient

#endif // EMAIL_ENABLE_INTERNAL_SSLCLIENT
