/* -*- C++ -*-
 * $ProjectHeader: volitve 0.7 Mon, 08 Sep 1997 17:37:41 +0200 andrej $
 *
 * $Id: State.cpp 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
 *
 * Hrani informacijo o stanju sprememb. Spremenjen je samo, èe je
 * nova ¹tevilka veèja od prej¹jne.
 */

#include "State.h"

State::State()
{
  val_ = 0;
  changed_ = 1;
}

void State::SetVal(unsigned int val)
{
  changed_ = val>val_;
  val_ = val;
}
  
int State::Changed()
{
  return changed_;
}
