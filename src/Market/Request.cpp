/*
 * $ProjectId$
 *
 * $Id: Request.cpp 1.2 Thu, 28 Aug 1997 21:38:14 +0000 andrej $
 *
 * Zahtevek.
 */

// Prièakujemo GNU String razred:
#include <String.h>
#include <ace/OS.h>

#include "Request.h"
#include "Config.h"

Request::Request()
{
  this->LastRC_ = 0;
  this->db = NULL;
}

Request::~Request()
{
  delete this->db;
}

int Request::LastRC()
{
  return this->LastRC_;
}

char *Request::Result()
{
  return this->Result_;
}

int Request::open()
{
  // Tole bi moral prebrati iz parametrov
  db = new PgDatabase(DB_NAME);

  if ( db->ConnectionBad() ) {
    ACE_ERROR_RETURN((LM_ERROR, "Error %s connecting to database %s\n", 
		      db->ErrorMessage(), DB_NAME), -1); 
  }
}

/* Procedura, ki vse opravi: */

int Request::Add(char *Line)
{
  /* Preberi zahtevek .. tale del bi moral prenesti v poseben objekt,
     ki bi se znal narediti tudi iz rezultata querija. */

  enum { KIND = 0, SYMBOL = 1, SHARES = 2, PRICE = 3, ACCOUNT = 4 };

  String words[5];

  // 0    1      2      3     4
  // KIND SYMBOL SHARES PRICE ACCOUNT
  int n = split(String(Line), words, 5, String(" "));

  if (n!=5) {
    LastRC_ = -1;
    ACE_ERROR_RETURN((LM_ERROR, "Wrong number of words\n"), LastRC_);
  }
  
  // BUY ali SELL
  // Zaenkrat uporabljamo kar tele funkcije - ne glede na Locale ipd.
  int Shares = atoi(words[SHARES].chars());      // Kolièina
  double Price = atof(words[PRICE].chars());     // Cena

  ACE_DEBUG((LM_DEBUG, "Request: V: %s, P: %s, S: %d, C: %f, R: %s\n",
	     words[KIND].chars(), words[SYMBOL].chars(), 
	     Shares, Price, words[ACCOUNT].chars()));
  
  /* Preveri veljavnost */
  
  if ((Price<=0) || (Price>100)) {
    ACE_ERROR_RETURN((LM_ERROR, "Price %f out of range\n", Price), -1);
  }
  
  /* Preveriti je treba ¹e: ali je papir veljaven, ali je raèun veljaven,
     ali ima raèun ¹e dovolj odprtih pozicij, ¹e kaj?*/ 

  /* Dodaj */

  /* Poi¹èemo vse kandidate: */

  char MatchFIFO[] = 
    "SELECT *\n"
    "FROM fifo\n"
    "WHERE\n"
    "     vrsta = '%s' and\n"
    "     cena <= %f and\n"
    "     papir_id = '%s'\n"
    "ORDER BY\n"
    "     cena, ura";

  int status;
  
  char statement[8192]; // 8K je omejitev v PostgreSQL, èe se ne motim
  
  sprintf(statement, MatchFIFO, "P", Price, words[SYMBOL].chars() );

  cout << statement << endl;
  
  if (status =  db->Exec(statement) != PGRES_TUPLES_OK) 
    ACE_ERROR_RETURN((LM_ERROR, "Error executing query: %d, %s \n",
		      status, db->ErrorMessage()),-1);

  db->DisplayTuples();
  
  char InsertPosli[] = "";

  char DeleteFIFO[] = "";

  char InsertFIFO[] = "";

  for (int i=0; i<db->Tuples() && Shares>0; i++) {
    /* Sklepaj posle: */
    
  } 
  
  
  strcpy(this->Result_, "OK");
  LastRC_ = 0;
  return LastRC_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Request, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Request, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

