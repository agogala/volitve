// -*- C++ -*-
/* 
 * $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
 *
 * $Id: Config.h 1.16 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
 *
 * Konfiguracija.
*/

#if !defined(CONFIG_H)
#define CONFIG_H

#if defined(DEHROSC)
/* Privzeta pot za administratorja */
#define ADMIN_DEFAULT_PATH "/tmp/admin_market-d"

/* Port za marketd (se trenutno ne uporablja) */
//#define MARKET_DEFAULT_PORT 5001

/* Privzeta pot za marketd */
#define MARKET_DEFAULT_PATH "/tmp/market-d"

/* Opazovalci uporabljajo tale port */
#define NOTIFIER_DEFAULT_PORT 6002

/* Tale port uporablja obve��evalec */
#define NOTIFIER_LOCAL_PORT 6001

/* Pot za opazovalce (se ne uprablja) */
//#define NOTIFIER_DEFAULT_PATH "/tmp/market_notify"

/* Pot za formaterja */
#define FORMATER_DEFAULT_PATH "/tmp/format_market-d"

/* Ime baze: */
#define DB_NAME "volitve_d"

#else
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

/* Ime baze: */
#define DB_NAME "volitve"
#endif

/* Velikosti polj: */
// Papirji:
#define MAX_PAPIR_ID 8

// Velikost ra�unov:
#define MAX_PONUDNIK 10

/* Velikost sporo�ila za opazovalce */
#define NOTIFIER_MESSAGE_LENGTH MAX_PONUDNIK + 12

// Velikost OID polja (vzeto iz kode za postgres (oidout v src/backend/utils/adt/oid.c) brez \0 stringa.
#define MAX_ID 11

/* Najve�ja dovoljena koli�ina */
#define MAX_KOL 25

/* �as zaprtja: */
#define CLOSING_TIME 880153200 // 'Sat Nov 22 00:00:00 1997' :-)))
//#define CLOSING_TIME 880117058

#endif
