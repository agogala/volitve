/*
 * $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
 * 
 * $Id: Utils.cpp 1.1 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
 *
 * Pomo¾ne funkcije.
 */

#include <stdlib.h>
#include <string.h>

// Pretvori celo ¹tevilo v znake
void int2code(int in, char *instr)
{
  int absin = abs(in);
  char *str = instr;

  for (int n=100; n>0; n /= 10) {
    *str = '0' + ((int)(absin / n)) % 10;
    str++;
  }
  *str = '\0';
}

// Pobri¹e zadnje presledke:
void rstrip(char *instr)
{
  int i = strlen(instr)-1;

  while(i>=0 && instr[i]==' ')
    i--;
  
  if (instr[i]==' ')
    instr[i]='\0';
  else
    instr[i+1]='\0';
}
