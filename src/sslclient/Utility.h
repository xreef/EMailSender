// Utility.h aggiornato dalla libreria ESP_SSLClient
// https://github.com/mobizt/ESP_SSLClient
#pragma once
#include <Arduino.h>

namespace sslclient {

class Utility {
public:
    static void hexlify(const uint8_t *src, size_t len, char *dest);
    static void printBuffer(const uint8_t *buf, size_t len);
};

} // namespace sslclient

