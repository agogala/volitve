/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: VectorRequest.h 1.2 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
 *
 * Enostaven vektor zahtevkov. 
 * STL se nekaj punta, vsaj na ro¾letu - g++, ACE, bog si ga vedi?
 * Definirani so samo ukazi, ki jih potrebujem pri trgu.
 */

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

