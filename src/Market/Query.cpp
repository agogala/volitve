/*
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: Query.cpp 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
 *
 * SQL queriji. V bistvu interface za sprintf.
 */
#include "Query.h"
//#include <stdarg.h>

extern "C" {
#include <stdio.h>
#include <stdarg.h>
	   }

Query::Query(char * q)
  : String(q)
{
}
  
const char * Query::Params(void * first, ...) const
{
  va_list ap;
  va_start(ap, first);

  // GNU extension: verjetno ne dela na Solarisu.
  //   vsnprintf(this->buff, this->chars(), ap);
  // Tu so lahko buffer povozi...?
  vsprintf(this->buff, this->chars(), ap);

  va_end(ap);

  return this->buff;
}
