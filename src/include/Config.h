// -*- C++ -*-
/* Konfiguracija */

#if !defined(CONFIG_H)
#define CONFIG_H

/* Port za marketd (se trenutno ne uporablja) */
#define MARKET_DEFAULT_PORT 5001

/* Privzeta pot za marketd */
#define MARKET_DEFAULT_PATH "/tmp/market"

/* Opazovalci uporabljajo tale port */
#define NOTIFIER_DEFAULT_PORT 5002

/* Pot za opazovalce (se ne uprablja) */
#define NOTIFIER_DEFAULT_PATH "/tmp/market_notify"

/* Velikost sporoèila za opazovalce */
#define NOTIFIER_MESSAGE_SIZE 9

/* Velikosti polj: */
// Papirji:
#define MAX_PAPIR_ID 4

// Velikost raèunov:
#define MAX_PONUDNIK 10

// Velikost OID polja (vzeto iz kode za postgres (oidout v src/backend/utils/adt/oid.c) brez \0 stringa.
#define MAX_ID 11

/* Ime baze: */
#define DB_NAME "volitve"

#endif
