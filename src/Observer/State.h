/* -*- C++ -*-
 * $ProjectHeader: volitve 0.8 Tue, 09 Sep 1997 00:58:50 -2200 andrej $
 *
 * $Id: State.h 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
 *
 * Hrani informacijo o stanju sprememb. Spremenjen je samo, �e je
 * nova �tevilka ve�ja od prej�jne.
 */

#if !defined(STATE_H)
#define STATE_H

class State 
{
 public:
  State();

  void SetVal(unsigned int val);
  int Changed();

 private:
  unsigned int val_;
  bool changed_;

};

#endif // STAT_H
