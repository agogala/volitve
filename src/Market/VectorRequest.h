/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.5 Thu, 04 Sep 1997 16:58:57 +0200 andrej $
 *
 * $Id: VectorRequest.h 1.1 Thu, 04 Sep 1997 02:15:14 +0000 andrej $
 *
 * Enostaven vektor zahtevkov. 
 * STL se nekaj punta, vsaj na ro¾letu - g++, ACE, bog si ga vedi?
 * Definirani so samo ukazi, ki jih potrebujem pri trgu.
 */

#include "Request.h"

class VectorRequest {
  Request *content;
  unsigned int Size_;
  unsigned int Filled_;
public:
  VectorRequest(unsigned int size);
  ~VectorRequest();

  void push_back(Request &r);
  unsigned int size();

  Request operator[](unsigned int i);

private:
  VectorRequest() {}

};
