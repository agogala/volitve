/*
 * $ProjectId$
 *
 * $Id: Request.cpp 1.1 Thu, 28 Aug 1997 17:15:39 +0000 andrej $
 *
 * Zahtevek.
 */

#include "Request.h"

Request::Request()
{
  LastRC_ = 0;
}

Request::~Request()
{
}

int Request::LastRC()
{
  return this->LastRC_;
}

char *Request::Result()
{
  return this->Result_;
}

/* Procedura, ki vse opravi: */

int Request::Add(char *Line)
{
  /* Preberi zahtevek */

  
  /* Preveri veljavnost */

  /* Dodaj */

  strcpy(this->Result_, "OK");
  LastRC_ = 0;
  return LastRC_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Request, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Request, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

