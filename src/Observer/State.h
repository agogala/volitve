/* -*- C++ -*-
 * $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
 *
 * $Id: State.h 1.1.2.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
 *
 * Hrani informacijo o stanju sprememb. Spremenjen je samo, ?e je
 * nova ?tevilka ve?ja od prej?jne.
 */

#if !defined(STATE_H)
#define STATE_H

#include <ace/Synch.h>

class State : public ACE_Test_and_Set <ACE_Null_Mutex, sig_atomic_t>
{
  /*
 public:
  State();

  void SetVal(unsigned int val);
  int Changed();

 private:
  unsigned int val_;
  bool changed_;
  */
};

#endif // STAT_H
