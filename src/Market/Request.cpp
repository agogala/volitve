/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve D.2 Wed, 03 Sep 1997 07:39:11 +0200 andrej $
 *
 * $Id: Request.cpp 1.1 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Zahtevek za blagovno borzo.
 *
 */

#include <String.h>
#include <ace/Log_Msg.h>
#include "Request.h"
#include "Query.h"

Request::Request()
{
  Valid_ = false;
}

Request::Request(char * rs)
{

  Valid_ = this->Read_i(rs) == 0;

}

// Preberi iz trenutne vrstice:
Request::Request(PgDatabase &db)
{
  Valid_ = this->Read_i(db);
}

Request::~Request()
{
}

bool Request::IsValid(PgDatabase &db) const
{
  if (this-> Valid_) {
    ((Request*)this)->Valid_ = (0 < this-> Cena_) && (this-> Cena_ <= 100);
    if (!this-> Valid_)
      ACE_ERROR_RETURN((LM_ERROR, "Price %f out of range\n", this->Cena_),-1);
    // Preveri kolièino: (to je potrebno ¹e razmisliti)

    // Itd...
  }
  return Valid_;
}
  
int Request::Store(PgDatabase &db) const
{
  // Èe ni veljaven zahtevek, zavrnemo shranjevanje...
  // Kako enostavno bi to bilo, èe bi lahko vrgel kak exception!
  if (!IsValid(db)) {
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid. Can't save\n"), -1);
  }

  Query insert(
    "INSERT INTO zahtevki"
    "VALUES ('%s', %f, %d, %s, TODAY, NOW, %s)");

  int status;

  if ((status = db.Exec
      (insert.Params(NULL, this->Vrsta_, this->Cena_, this->Kolicina_, 
		     this->Papir_ID_, this->Ponudnik_))) != PGRES_COMMAND_OK) {
    ACE_ERROR_RETURN((LM_ERROR, "Error inserting requests: %d, %s\n",
		      status, db.ErrorMessage()),-1);
  }

  return 0;
}

const char *Request::Ponudnik() const
{
  return this->Ponudnik_;
}

const char *Request::Papir_ID() const
{
  return this->Papir_ID_;
}

// Vrne negativno vrednost, ce je nakup, pozitivno ce je prodaja.
unsigned int Request::Kolicina() const
{
  return this->Kolicina_;
}

double Request::Cena() const
{
  return this->Cena_ * (this->Vrsta_ == 'N' ? -1 : 1) ;
}

// Vrne razlog zakaj zadeva ni veljavna
/*char *Request::LastError() const
{
  return this->LastError_;
}*/

int Request::Read_i(PgDatabase &db)
{
  return 0;
}

int Request::Read_i(char * rs)
{

  Valid_ = false;

  // Err, tole je precej kruto narejeno:
  enum { KIND = 0, SYMBOL = 1, SHARES = 2, PRICE = 3, ACCOUNT = 4 };

  String words[5];

  // 0    1      2      3     4
  // KIND SYMBOL SHARES PRICE ACCOUNT
  int n = split(String(rs), words, 5, String(" "));

  if (n!=5) {
    ACE_ERROR_RETURN((LM_ERROR, "Wrong number of words"), -1);
  }
  
  // BUY ali SELL
  // Zaenkrat uporabljamo kar tele funkcije - ne glede na Locale ipd.
  this->Kolicina_ = atoi(words[SHARES].chars());      // Kolièina
  this->Cena_ = atof(words[PRICE].chars());     // Cena
  this->Vrsta_ = words[KIND][0] == 'B' ? 'N' : 'P';
  strcpy(this->Ponudnik_, words[ACCOUNT].chars());
  strcpy(this->Papir_ID_, words[SYMBOL].chars());

  return 0;
}
