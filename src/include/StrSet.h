/* -*- C++ -*-
 *
 * $ProjetcHeader$
 *
 * $Id: StrSet.h 1.2 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
 *
 * Definira mno¾ico stringov.
 */

#if !defined(STRSET_H)
#define STRSET_H

#include <set.h>
#include "Utils.h"

typedef set<const char*, ltstr> strset;

#endif
