/*
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: VectorRequest.cpp 1.4 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
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


