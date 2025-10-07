// Example of sending email via GMail SMTP with EMailSender and SSLClient (ESP_SSLClient)
// Works on both 465 (direct SSL/TLS) and 587 (STARTTLS)
//
// Prerequisites:
// - To use STARTTLS on 587 enable in EMailSenderKey.h:
//     #define EMAIL_ENABLE_INTERNAL_SSLCLIENT
//   (or add the build flag: -DEMAIL_ENABLE_INTERNAL_SSLCLIENT)
// - For implicit TLS on 465 on ESP32 nothing extra is required.
#include <WiFi.h>
#include "EMailSender.h"
#include "sslclient/CertStore.h"
#include "sslclient/SSLClient.h"

// ...existing code...
