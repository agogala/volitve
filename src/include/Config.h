// -*- C++ -*-
/* 
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: Config.h 1.13 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
 *
 * Konfiguracija.
*/

#if !defined(CONFIG_H)
#define CONFIG_H

/* Privzeta pot za administratorja */
#define ADMIN_DEFAULT_PATH "/tmp/admin_market"

/* Port za marketd (se trenutno ne uporablja) */
//#define MARKET_DEFAULT_PORT 5001

/* Privzeta pot za marketd */
#define MARKET_DEFAULT_PATH "/tmp/market"

/* Opazovalci uporabljajo tale port */
#define NOTIFIER_DEFAULT_PORT 5002

/* Tale port uporablja obve��evalec */
#define NOTIFIER_LOCAL_PORT 5001

/* Pot za opazovalce (se ne uprablja) */
//#define NOTIFIER_DEFAULT_PATH "/tmp/market_notify"

/* Pot za formaterja */
#define FORMATER_DEFAULT_PATH "/tmp/format_market"

/* Velikosti polj: */
// Papirji:
#define MAX_PAPIR_ID 8

// Velikost ra�unov:
#define MAX_PONUDNIK 10

/* Velikost sporo�ila za opazovalce */
#define NOTIFIER_MESSAGE_LENGTH MAX_PONUDNIK + 9

// Velikost OID polja (vzeto iz kode za postgres (oidout v src/backend/utils/adt/oid.c) brez \0 stringa.
#define MAX_ID 11

/* Ime baze: */
#define DB_NAME "volitve"

/* Najve�ja dovoljena koli�ina */
#define MAX_KOL 25

#endif
