/* -*- C++ -*-
 *
 * $ProjetcHeader$
 *
 * $Id: StrSet.h 1.1 Thu, 25 Sep 1997 19:32:05 +0000 andrej $
 *
 * Definira mno¾ico stringov.
 */

#if !defined(STRSET_H)
#define STRSET_H

#include <set.h>

struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};
  
typedef set<const char*, ltstr> strset;

#endif
