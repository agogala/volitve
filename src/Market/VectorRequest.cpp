/*
 * $ProjectHeader: volitve 0.7 Mon, 08 Sep 1997 17:37:41 +0200 andrej $
 *
 * $Id: VectorRequest.cpp 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
 *
 * Implementacija enostavnega vektorja -- STL, ACE in g++ na Rozletu
 * ne gredo skupaj.
 *
 */

#include "VectorRequest.h"

VectorRequest::VectorRequest(unsigned int size)
{
  content = new Request[size];
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

