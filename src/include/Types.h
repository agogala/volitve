/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.4 Thu, 04 Sep 1997 04:15:14 +0200 andrej $
 *
 * $Id: Types.h 1.1 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Blagovna borza. Razlièni tipi.
 *
 */

#if !defined (TYPES_H)
#define TYPES_H

/*
extern "C" {
#include <time.h>
	   }
	   */

// Tole mora ¾e kje obstajati:
class date {
 public:
  // remembers current date
  date();
  date(char * indate);
  ~date();

  char *SQLDate() const;

 private:
  
}

class time {
 public:
  
 private:
  
}

#endif /* TYPES_H */
