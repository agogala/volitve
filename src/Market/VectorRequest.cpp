/*
 * $ProjectHeader: volitve 0.16 Tue, 30 Sep 1997 18:27:41 +0200 andrej $
 *
 * $Id: VectorRequest.cpp 1.3 Tue, 09 Sep 1997 22:58:50 +0000 andrej $
 *
 * Implementacija enostavnega vektorja -- STL, ACE in g++ na Rozletu
 * ne gredo skupaj.
 *
 */

#include "VectorRequest.h"

VectorRequest::VectorRequest(unsigned int size)
{
  if (size!=0)
    content = new Request[size];
  else
    content = NULL;
  Size_ = size;
  Filled_ = 0;
}

VectorRequest::~VectorRequest()
{
  delete content;
}

void VectorRequest::push_back(Request &r)
{
  if (Filled_<Size_) {
    content[Filled_] = r;
    Filled_++;
  } // Sicer pa molèe prezri zadevo...
}

unsigned int VectorRequest::size()
{
  return this->Size_;
}

Request VectorRequest::operator[](unsigned int i)
{
  if (i<Filled_) 
    return content[i];
  else // Sicer pa vrnemo kar zadnjega ... kako lahko vrnem napako?
    return content[Filled_-1];
}

