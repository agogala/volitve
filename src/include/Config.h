// -*- C++ -*-
/* 
 * $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
 *
 * $Id: Config.h 1.14 Tue, 04 Nov 1997 18:56:32 +0000 andrej $
 *
 * Konfiguracija.
*/

#if !defined(CONFIG_H)
#define CONFIG_H

#if defined(DEBUG)
/* Privzeta pot za administratorja */
#define ADMIN_DEFAULT_PATH "/tmp/admin_market-d"

/* Port za marketd (se trenutno ne uporablja) */
//#define MARKET_DEFAULT_PORT 5001

/* Privzeta pot za marketd */
#define MARKET_DEFAULT_PATH "/tmp/market-d"

/* Opazovalci uporabljajo tale port */
#define NOTIFIER_DEFAULT_PORT 6002

/* Tale port uporablja obve¹èevalec */
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

/* Tale port uporablja obve¹èevalec */
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

// Velikost raèunov:
#define MAX_PONUDNIK 10

/* Velikost sporoèila za opazovalce */
#define NOTIFIER_MESSAGE_LENGTH MAX_PONUDNIK + 12

// Velikost OID polja (vzeto iz kode za postgres (oidout v src/backend/utils/adt/oid.c) brez \0 stringa.
#define MAX_ID 11

/* Najveèja dovoljena kolièina */
#define MAX_KOL 25

#endif
