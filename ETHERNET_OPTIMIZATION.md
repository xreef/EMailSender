# Ottimizzazione Ethernet per Arduino Mega con SSL/TLS

## Problema
Arduino Mega ha solo 8KB di RAM, ma BearSSL + Ethernet richiedono molta memoria.

## Ottimizzazioni applicate in EMailSenderKey.h:
✅ `ENABLE_ATTACHMENTS` = DISABILITATO (risparmia ~1-2KB)
✅ `EMAIL_SENDER_DEBUG` = DISABILITATO (risparmia ~500 bytes per stringhe debug)
✅ `EMAIL_ENABLE_OPENSLAB_SSLCLIENT` = ABILITATO (necessario per SSL/TLS)

## Ottimizzazione libreria Ethernet

### Opzione 1: Ridurre buffer socket nel tuo sketch
Aggiungi PRIMA di `#include <Ethernet.h>` nel tuo sketch:

```cpp
// Riduci i buffer Ethernet da 2KB a 1KB per socket
#define MAX_SOCK_NUM 4          // Usa solo 4 socket invece di 8
#define ETHERNET_LARGE_BUFFERS 0 // Disabilita buffer grandi

#include <Ethernet.h>
#include <SD.h>
#include <EMailSender.h>
```

### Opzione 2: Modificare w5100.h della libreria Ethernet
Percorso: `C:\Users\renzo\AppData\Local\Arduino15\libraries\Ethernet\src\utility\w5100.h`

Cerca queste righe e modificale:
```cpp
// PRIMA (default):
#define MAX_SOCK_NUM 8
static uint16_t SSIZE = 2048; // buffer 2KB per socket

// DOPO (ottimizzato):
#define MAX_SOCK_NUM 4         // Solo 4 socket
static uint16_t SSIZE = 1024;  // Buffer 1KB per socket
```

**Risparmio stimato**: ~4-8KB di RAM

### Opzione 3: Usare EthernetLarge (consigliato per SSL)
La libreria **OPEnSLab-OSU/EthernetLarge** è ottimizzata per funzionare con SSLClient.

Installazione:
1. Scarica da: https://github.com/OPEnSLab-OSU/EthernetLarge
2. Installa nel Library Manager Arduino
3. Usa `#include <EthernetLarge.h>` invece di `#include <Ethernet.h>`

## Altre ottimizzazioni possibili:

### 1. Ridurre certificati trust_anchors
Nel file `trust_anchors.h`, mantieni SOLO i certificati necessari per il tuo server SMTP.

### 2. Usare stringhe PROGMEM
Le stringhe costanti vanno in Flash invece che in RAM:
```cpp
const char server[] PROGMEM = "smtp.example.com";
```

### 3. Disabilitare funzionalità SD se non servono
Se non usi SD nel tuo progetto, rimuovi `#include <SD.h>` dallo sketch.

## Test di memoria
Dopo ogni ottimizzazione, controlla l'output di compilazione:
- **Flash**: max 253952 bytes (253KB)
- **RAM globale**: max 8192 bytes (8KB) ← CRITICO
- **RAM locale**: variabili runtime

Se la RAM globale supera 8KB, la compilazione fallisce.

