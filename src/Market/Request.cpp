/* -*- C++ -*- */
/*
 * $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
 *
 * $Id: Request.cpp 1.9 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
 *
 * Zahtevek za blagovno borzo.
 *
 */

//#include <String.h>
#include <ace/OS.h>
#include <ace/Process.h>
#include <ace/Log_Msg.h>
#include "Request.h"
#include "Query.h"
#include "MarketErrors.h"
#include "Utils.h"
#include "Market.h"

Request::Request()
{
  Valid_ = false;
  ACE_OS::memset(&ID_, '\0', sizeof(ID_));
  ACE_OS::memset(&Ponudnik_, '\0', sizeof(Ponudnik_));
  ACE_OS::memset(&Papir_ID_, '\0', sizeof(Papir_ID_));
  Kolicina_ = 0;
  Cena_ = 0;
  LastError_ = me_OK;
}

Request::Request(char * rs)
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

bool Request::IsValid(PgDatabase &db)
{
  if (this->Preklic_) {
    // Preklic, preveri èe v FIFO obstaja zahtevek s takim ID:

    // Poi¹èi zahtevek v FIFO:    
    // Parameter:
    // OID:
    Query FindFIFO=
      "SELECT *\n"
      "FROM FIFO\n"
      "WHERE oid=%s\n";

    if (this->Valid_) {
      if (!db.Exec(FindFIFO.Params(NULL, this->ID_))) {
	  ((Request*)this)->LastError_ = me_InternalError;	
	  ((Request*)this)->Valid_ = false;      
	  ACE_ERROR_RETURN((LM_ERROR, "Error lookin for request: '%s' \n",
			    db.ErrorMessage()), this-> Valid_);
      }
      ((Request*)this)->Valid_ = ((Request*)this->Valid_) &&
	db.Tuples()==1;
      if (!this->Valid_) {
	((Request*)this)->LastError_ = me_NoSuchRequest;
	ACE_ERROR_RETURN((LM_ERROR, "No such request '%s'\n", 
			  this-> ID_), this->Valid_);
      }
    }
  } else { // Ni Preklic:
    if (this-> Valid_) {
      // Preveri papir:
      Query CheckPapir=
	"SELECT DISTINCT Papir_id\n"
	"FROM Papirji\n"
	"WHERE Papir_id='%s'";
      // Ni prazen string:
      ((Request*)this)->Valid_ = Papir_ID_[0]!='\0';
      if (this-> Valid_)
	if (!db.Exec(CheckPapir.Params(NULL, this->Papir_ID_))) {
	  ((Request*)this)->LastError_ = me_InternalError;	
	  ((Request*)this)->Valid_ = false;      
	  ACE_ERROR_RETURN((LM_ERROR, "Error checking symbol: '%s' \n",
			    db.ErrorMessage()), this-> Valid_);
	
	}
      ((Request*)this)->Valid_ = ((Request*)this->Valid_) &&
	db.Tuples()>0;
      if (!this-> Valid_) {
	((Request*)this)->LastError_ = me_NoSuchSymbol;
	ACE_ERROR_RETURN((LM_ERROR, "No such symbol '%s'\n", 
			  this-> Papir_ID_), this->Valid_);
      }
		    
      // Preveri stranko:
      Query CheckStranka=
	"SELECT DISTINCT Stranka_id\n"
	"FROM Stranke\n"
	"WHERE Stranka_id='%s'";
      // Ni prazen string:
      ((Request*)this)->Valid_ = Ponudnik_[0]!='\0';
      if (this-> Valid_)
	if (!db.Exec(CheckStranka.Params(NULL, this->Ponudnik_))) {
	  ((Request*)this)->LastError_ = me_InternalError;	
	  ((Request*)this)->Valid_ = false;
	  ACE_ERROR_RETURN((LM_ERROR, "Error checking user: '%s' \n",
			    db.ErrorMessage()), this-> Valid_);
	
	}
      this ->Valid_ = this->Valid_ && db.Tuples()>0;
      if (!(this-> Valid_)) {
	this ->LastError_ = me_NoSuchUser;
	ACE_ERROR_RETURN((LM_ERROR, "No such user '%s'\n",
			  this-> Ponudnik_), this-> Valid_);
      }

      // Preveri ceno:
      ((Request*)this)->Valid_ = (0 < this-> Cena_) && (this-> Cena_ <= 100);
      if (!this-> Valid_) {
	((Request*)this)->LastError_ = me_WrongPrice;
	ACE_ERROR_RETURN((LM_ERROR, "Price %f out of range\n", this->Cena_),
			 this->Valid_);
      }

    }
    
  }
  
  return this->Valid_;
}
  
// Preberi iz tabele:
int Request::Read(PgDatabase &db, const int tup_num)
{
  return Read_i(db, tup_num);
}


int Request::Store(PgDatabase &db) 
{
  LastError_ = me_OK;

  // Èe ni veljaven zahtevek, zavrnemo shranjevanje...
  // Kako enostavno bi to bilo, èe bi lahko vrgel kak exception!
  if (!IsValid(db)) {
    LastError_ = me_InternalError;
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid. Can't store.\n"), LastError_);
  }

  if (this->Preklic_) {
    // Prekopiraj iz FIFO:
    // Parameter:
    // oid:
    Query copy=
      "INSERT INTO zahtevki\n"
      "SELECT cena, kolicina,\n"
      "       papir_id, 'today' AS datum,\n"
      "       'now' AS ura, ponudnik,\n"
      "       't' AS preklic\n"
      "WHERE oid=%s";

    if (!db.ExecCommandOk
	(copy.Params(NULL, this->ID_))) {
      LastError_ = me_InternalError;
      ACE_ERROR_RETURN((LM_ERROR, "Error inserting cancel request: %s\n",
			db.ErrorMessage()), LastError_);
    }
  } else {

    /* Vstavi v zahtevke: 
     Parametri:
     Cena
     Kolicina
     Papir_ID
     Ponudnik
   */
    Query insert(
		 "INSERT INTO zahtevki\n"
		 "\tVALUES (%f, %d, '%s', 'today', 'now', '%s', %d)");

    if (!db.ExecCommandOk
	(insert.Params(NULL, this->Cena_, this->Kolicina_, 
		       this->Papir_ID_, this->Ponudnik_, (int)this->Preklic_))) {
      LastError_ = me_InternalError;
      ACE_ERROR_RETURN((LM_ERROR, "Error inserting requests: %s\n",
			db.ErrorMessage()), LastError_);
    }

    ACE_OS::strcpy(this->ID_, db.OidStatus());

  }
  return LastError_;
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

bool Request::Preklic() const
{
  return this->Preklic_;
}

const char *Request::ID() const
{
  return this->ID_;
}

// Omejitev:
int
Request::Omeji(PgDatabase &db)
{
  // Preveri napake [...]
  Stanje *s;
  s = (*(STANJA::instance() ->Get(db, Ponudnik_))).second;

  // Zaznamek o spremembi se shrani drugje
  Kolicina_ = s->Omeji(db, Papir_ID_, Kolicina_);

  return 0;
}
  
// Sprememba zahtevka. Samo v spominu.
int
Request::Spremeni(int Kolicina)
{
  this->Kolicina_ = Kolicina;
  return 0;
}


int Request::LastError() const
{
  return this->LastError_;
}

int Request::Read_i(PgDatabase &db, const int tup_num)
{
  
  LastError_ = me_OK;

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

  if (!Valid_) {
    LastError_ = me_InternalError;
    ACE_ERROR_RETURN((LM_ERROR, "Missing fields in table\n"), LastError_);
  }

  this->Kolicina_ = atoi(db.GetValue(tup_num, indKolicina));      // Kolièina
  this->Cena_ = atof(db.GetValue(tup_num, indCena));     // Cena
  //  this->Vrsta_ = db.GetValue(tup_num, indVrsta)[0];
  // Strip white space from values:
  ACE_OS::strcpy(this->Ponudnik_, db.GetValue(tup_num, indPonudnik));
  rstrip(this->Ponudnik_);
  ACE_OS::strcpy(this->Papir_ID_, db.GetValue(tup_num, indPapir_ID));
  rstrip(this->Papir_ID_);
  ACE_OS::strcpy(this->ID_, db.GetValue(tup_num, indID));
  rstrip(this->ID_);

  this->Preklic_ = false;

  return this->LastError_;
}

int Request::Read_i(char * rs)
{

  Valid_ = false;
  LastError_ = me_OK;

  char buff[256];

  strncpy(buff, rs, 255);
  buff[255] = '\0';

  ACE_Tokenizer tokens(buff);
  tokens.delimiter_replace(' ', '\0');
  tokens.preserve_designators('"','"');
  
  char *cmd = tokens.next();

  //  ACE_DEBUG((LM_DEBUG, "CMD: %s\n", cmd));
  
  if (!ACE_OS::strcasecmp(cmd, "Buy") || 
      !ACE_OS::strcasecmp(cmd, "Sell")) {
    this->Preklic_ = false;
    //    ACE_DEBUG((LM_DEBUG, "SELL or BUYqn"));
    ACE_OS::strcpy(this->Papir_ID_, tokens.next());
    //    ACE_DEBUG((LM_DEBUG, "PAPER: %s\n", Papir_ID_));
    this->Kolicina_ = atoi(tokens.next()) * (toupper(cmd[0]) == 'S' ? -1 : 1);
    this->Cena_ = atof(tokens.next());
    ACE_OS::strcpy(this->Ponudnik_, tokens.next());

    ACE_OS::memset(&ID_, '\0', sizeof(ID_));

  } else if (!ACE_OS::strcasecmp(cmd, "Cancel")) {
    // Ignore for the moment
    this->Preklic_ = true;
    // Doloèimo ostale parametre:
    ACE_OS::strcpy(this->ID_, tokens.next());
    ACE_OS::strcpy(this->Ponudnik_, tokens.next());
  } else {
    // Error
    this->LastError_ = me_SyntaxError;
  }
  
  return this->LastError_;

}

