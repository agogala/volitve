// -*- C++ -*-
/* Konfiguracija */

#if !defined(CONFIG_H)
#define CONFIG_H

/* Default port for market connections */
#define MARKET_DEFAULT_PORT 5001

/* Default market LSOCK path */
#define MARKET_DEFAULT_PATH "/tmp/market"

/* Default port for observer connections */
#define NOTIFIER_DEFAULT_PORT 5002

/* Default market LSOCK path */
#define NOTIFIER_DEFAULT_PATH "/tmp/market_notify"

/* Define broadcast message size */
#define NOTIFIER_MESSAGE_SIZE 9

/* Some sizes */
#define MAX_KIND 4

#define MAX_SYMBOL 4

#define MAX_ACCOUNT 8

/* Default database name */
#define DB_NAME "volitve"

#endif
