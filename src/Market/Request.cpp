/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
 *
 * $Id: Request.cpp 1.3 Tue, 09 Sep 1997 22:58:50 +0000 andrej $
 *
 * Zahtevek za blagovno borzo.
 *
 */

#include <String.h>
#include <ace/OS.h>
#include <ace/Log_Msg.h>
#include "Request.h"
#include "Query.h"

Request::Request()
{
  Valid_ = false;
  ACE_OS::memset(&ID_, '\0', sizeof(ID_));
  ACE_OS::memset(&Ponudnik_, '\0', sizeof(Ponudnik_));
  ACE_OS::memset(&Papir_ID_, '\0', sizeof(Papir_ID_));
  Kolicina_ = 0;
  Cena_ = 0;
}

Request::Request(const char * rs)
{

  Valid_ = this->Read_i(rs) == 0;

}

// Preberi iz trenutne vrstice:
Request::Request(PgDatabase &db, const int tup_num)
{
  Valid_ = this->Read_i(db, tup_num) == 0;
}

/*
Request::Request(Request &req)
{
  ACE_OS::strcpy(Ponudnik_, req.Ponudnik());
  ACE_OS::strcpy(Papir_ID_, req.Papir_ID());
  
  Kolicina_ = req.Kolicina();
  Cena_ = req.Cena();

  Valid_ = req.Valid_;

  ACE_OS::strcpy(ID_, req.ID_);
}
*/

Request::~Request()
{
}

bool Request::IsValid(PgDatabase &db) const
{
  if (this-> Valid_) {
    ((Request*)this)->Valid_ = (0 < this-> Cena_) && (this-> Cena_ <= 100);
    if (!this-> Valid_)
      ACE_ERROR_RETURN((LM_ERROR, "Price %f out of range\n", this->Cena_),
		       this->Valid_);
    // Preveri kolièino: (to je potrebno ¹e razmisliti)

    // Itd...
  }
  return Valid_;
}
  
// Preberi iz tabele:
int Request::Read(PgDatabase &db, const int tup_num)
{
  return Read_i(db, tup_num);
}


int Request::Store(PgDatabase &db) 
{
  // Èe ni veljaven zahtevek, zavrnemo shranjevanje...
  // Kako enostavno bi to bilo, èe bi lahko vrgel kak exception!
  if (!IsValid(db)) {
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid. Can't store.\n"), -1);
  }

  /* Vstavi v zahtevke: 
     Parametri:
     Cena
     Kolicina
     Papir_ID
     Ponudnik
   */

  Query insert(
    "INSERT INTO zahtevki\n"
    "\tVALUES (%f, %d, '%s', 'today', 'now', '%s')");

  if (!db.ExecCommandOk
      (insert.Params(NULL, this->Cena_, this->Kolicina_, 
		     this->Papir_ID_, this->Ponudnik_))) {
    ACE_ERROR_RETURN((LM_ERROR, "Error inserting requests: %s\n",
		      db.ErrorMessage()),-1);
  }

  ACE_OS::strcpy(this->ID_, db.OidStatus());

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
int Request::Kolicina() const
{
  return this->Kolicina_;
}

double Request::Cena() const
{
  return this->Cena_;
}

const char *Request::ID() const
{
  return this->ID_;
}


// Vrne razlog zakaj zadeva ni veljavna
/*char *Request::LastError() const
{
  return this->LastError_;
}*/

int Request::Read_i(PgDatabase &db, const int tup_num)
{
  
  // Predpostavimo, da je iskanje po imenih dolgotrajnej¹e, zato raje 
  // izraèunamo indekse kar na zaèetku. Hkrati preverimo, èe so vsa polja
  // na voljo:
  //  int indVrsta = db.FieldNum("vrsta");
  int indPonudnik = db.FieldNum("ponudnik");
  int indPapir_ID = db.FieldNum("papir_id");
  int indKolicina = db.FieldNum("kolicina");
  int indCena = db.FieldNum("cena");
  int indID = db.FieldNum("oid");

  Valid_ = (indPonudnik>=0) &&
           (indKolicina>=0) && (indCena>=0) &&
           (indID>=0) && (indPapir_ID>=0);

  if (!Valid_) 
    ACE_ERROR_RETURN((LM_ERROR, "Missing fields in table\n"), -1);

  this->Kolicina_ = atoi(db.GetValue(tup_num, indKolicina));      // Kolièina
  this->Cena_ = atof(db.GetValue(tup_num, indCena));     // Cena
  //  this->Vrsta_ = db.GetValue(tup_num, indVrsta)[0];
  ACE_OS::strcpy(this->Ponudnik_, db.GetValue(tup_num, indPonudnik));
  ACE_OS::strcpy(this->Papir_ID_, db.GetValue(tup_num, indPapir_ID));
  ACE_OS::strcpy(this->ID_, db.GetValue(tup_num, indID));

  return 0;
}

int Request::Read_i(const char * rs)
{

  Valid_ = false;

  // Err, tole je precej kruto narejeno:
  enum { KIND = 0, SYMBOL = 1, SHARES = 2, PRICE = 3, ACCOUNT = 4 };

  String words[5];

  // 0    1      2      3     4
  // KIND SYMBOL SHARES PRICE ACCOUNT
  int n = split(String(rs), words, 5, String(" "));

  if (n!=5) {
    ACE_ERROR_RETURN((LM_ERROR, "Wrong number of words: %d\n", n), -1);
  }
  
  // BUY ali SELL
  // Zaenkrat uporabljamo kar tele funkcije - ne glede na Locale ipd.
  this->Kolicina_ = atoi(words[SHARES].chars()) 
    * (words[KIND][0] == 'B' ? -1 : 1); // Kolièina
  this->Cena_ = atof(words[PRICE].chars());     // Cena
  //  this->Vrsta_ = words[KIND][0] == 'B' ? 'N' : 'P';
  ACE_OS::strcpy(this->Ponudnik_, words[ACCOUNT].chars());
  ACE_OS::strcpy(this->Papir_ID_, words[SYMBOL].chars());

  ACE_OS::memset(&ID_, '\0', sizeof(ID_));

  return 0;
}
