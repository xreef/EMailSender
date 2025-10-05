// Utility.cpp aggiornato dalla libreria ESP_SSLClient
// https://github.com/mobizt/ESP_SSLClient

// Questo file deve essere compilato SOLO se EMAIL_ENABLE_INTERNAL_SSLCLIENT è abilitato
// Ultima modifica: forza rebuild per applicare le guard
#if defined(EMAIL_ENABLE_INTERNAL_SSLCLIENT)

#include "Utility.h"

namespace sslclient {

void Utility::hexlify(const uint8_t *src, size_t len, char *dest) {
    static const char hex[] = "0123456789abcdef";
    for (size_t i = 0; i < len; i++) {
        dest[i * 2] = hex[(src[i] >> 4) & 0xF];
        dest[i * 2 + 1] = hex[src[i] & 0xF];
    }
    dest[len * 2] = '\0';
}

void Utility::printBuffer(const uint8_t *buf, size_t len) {
#if defined(ESP8266) || defined(ESP32)
    for (size_t i = 0; i < len; i++) {
        Serial.printf("%02X ", buf[i]);
        if ((i + 1) % 16 == 0)
            Serial.println();
    }
    Serial.println();
#else
    for (size_t i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
#endif
}

} // namespace sslclient

#endif // EMAIL_ENABLE_INTERNAL_SSLCLIENT
