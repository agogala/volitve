/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.12 Mon, 22 Sep 1997 15:21:03 +0200 andrej $
 *
 * $Id: Query.h 1.1 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Razred, ki oblikuje SQL querije.
 *
 */

#if !defined (QUERY_H)
#define QUERY_H

#include <String.h>

class Query : public String
{
 public:
  Query(char * q);
  
  const char * Params(void * first, ...) const;

 private:
  char buff[8193]; // Precej potratno... PostgreSQL dovoli 8K najveè za query.
};

#endif // QUERY_H
