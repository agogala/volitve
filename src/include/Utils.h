/* -*- C++ -*-
 * $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
 *
 * $Id: Utils.h 1.2 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
 *
 * Pomo¾ne funckije.
 */

#if !defined(UTILS_H)
#define UTILS_H

// Pretvori ¹tevilo v 3 mesten string
void int2code(int in, char *instr);

// Pobri¹i zadnje presledke.
void rstrip(char *instr);


struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};

#endif
