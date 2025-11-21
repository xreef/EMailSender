/*
 * EMailSender_OpenSLabUnity.cpp
 *
 *  Created on: 29 set 2021
 *      Author: gualt
 *
 *  This file is a "unity build" file.
 *  It is used to compile all the necessary source files for the OpenSLab SSL client.
 *  This is necessary because the Arduino IDE has limitations on how it discovers and compiles source files in subdirectories.
 */

// NOTE: Do NOT force-enable EMAIL_ENABLE_OPENSLAB_SSLCLIENT here.
// The feature should be enabled explicitly in EMailSenderKey.h or via build flags
// (for example -DEMAIL_ENABLE_OPENSLAB_SSLCLIENT) so users control whether
// the OpenSLab SSL client backend is compiled.

#if defined(EMAIL_ENABLE_OPENSLAB_SSLCLIENT) && !defined(EMAIL_ENABLE_EXTERNAL_SSLCLIENT_OPENSLAB)

// Includiamo solo i file SSLClient necessari
// Non includiamo TLS12_only_profile.c che viene già compilato separatamente
#include "sslclientosu/third_party/SSLClient.cpp.inc"
#include "sslclientosu/third_party/SSLClientParameters.cpp.inc"

// Definiamo qui il simbolo mancante br_ec_prime_fast_256
extern "C" {
  #include "sslclientosu/third_party/bearssl_ec.h"

  // Usa br_ec_p256_m15 che è già disponibile come simbolo
  // br_ec_prime_fast_256 è solo un alias per br_ec_p256_m15
  const br_ec_impl br_ec_prime_fast_256 = br_ec_p256_m15;
}

#endif // EMAIL_ENABLE_OPENSLAB_SSLCLIENT
