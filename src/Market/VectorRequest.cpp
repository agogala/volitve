/*
 * $ProjectHeader: volitve 0.19 Thu, 09 Oct 1997 15:19:34 +0200 andrej $
 *
 * $Id: VectorRequest.cpp 1.5 Sun, 05 Oct 1997 20:53:12 +0000 andrej $
 *
 * Implementacija enostavnega vektorja -- STL, ACE in g++ na Rozletu
 * ne gredo skupaj.
 *
 */

#include "VectorRequest.h"

// VectorRequest dummyVR;


// #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
// template class vector<Request*>;
// #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
// #pragma instantiate vector<Request*>
// #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */



VectorRequest::VectorRequest(unsigned int size)
{
  if (size!=0)
    content = new (Request*)[size];
  else
    content = NULL;
  Size_ = size;
  Filled_ = 0;
}

VectorRequest::~VectorRequest()
{
  for (int i=0; i<Filled_; i++)
    delete content[i];
  delete content;
}

void VectorRequest::push_back(Request *r)
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

Request *VectorRequest::operator[](unsigned int i)
{
  if (i<Filled_) 
    return content[i];
  else // Sicer pa vrnemo kar zadnjega ... kako lahko vrnem napako?
    return content[Filled_-1];
}


