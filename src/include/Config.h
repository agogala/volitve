// -*- C++ -*-
/* 
 * $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
 *
 * $Id: Config.h 1.8 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
 *
 * Konfiguracija.
*/

#if !defined(CONFIG_H)
#define CONFIG_H

/* Port za marketd (se trenutno ne uporablja) */
//#define MARKET_DEFAULT_PORT 5001

/* Privzeta pot za marketd */
#define MARKET_DEFAULT_PATH "/tmp/market"

/* Opazovalci uporabljajo tale port */
#define NOTIFIER_DEFAULT_PORT 5002

/* Tale port uporablja notifier */
#define NOTIFIER_LOCAL_PORT 5001

/* Pot za opazovalce (se ne uprablja) */
//#define NOTIFIER_DEFAULT_PATH "/tmp/market_notify"

/* Velikost sporoèila za opazovalce */
#define NOTIFIER_MESSAGE_LENGTH 8

/* Velikosti polj: */
// Papirji:
#define MAX_PAPIR_ID 8

// Velikost raèunov:
#define MAX_PONUDNIK 10

// Velikost OID polja (vzeto iz kode za postgres (oidout v src/backend/utils/adt/oid.c) brez \0 stringa.
#define MAX_ID 11

/* Ime baze: */
#define DB_NAME "volitve"

#endif
