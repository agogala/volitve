/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
 *
 * $Id: VectorRequest.h 1.3 Sun, 05 Oct 1997 20:53:12 +0000 andrej $
 *
 * Enostaven vektor zahtevkov. 
 * STL se nekaj punta, vsaj na ro¾letu - g++, ACE, bog si ga vedi?
 * Definirani so samo ukazi, ki jih potrebujem pri trgu.
 */

#if !defined(VECTORREQUEST_H)
#define VECTORREQUEST_H

#include "Request.h"
//#include <vector.h>

//typedef vector<Request*> VectorRequest ;


class VectorRequest {
  Request **content;
  unsigned int Size_;
  unsigned int Filled_;
public:
  VectorRequest(unsigned int size);
  ~VectorRequest();

  void push_back(Request *r);
  unsigned int size();

  Request * operator[](unsigned int i);

private:
  VectorRequest() {}

};

#endif
