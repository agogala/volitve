/*
 * $ProjectHeader: volitve D.2 Wed, 03 Sep 1997 07:39:11 +0200 andrej $
 *
 * $Id: Market.cpp 1.3 Wed, 03 Sep 1997 05:39:11 +0000 andrej $
 *
 * Zahtevek.
 */

// Prièakujemo GNU String razred:
#include <ace/OS.h>

#include "Market.h"
#include "Request.h"
#include "Config.h"

Market::Market()
{
  this->LastRC_ = 0;
  this->db = NULL;
}

Market::~Market()
{
  delete this->db;
}

int Market::LastRC()
{
  return this->LastRC_;
}

char *Market::Result()
{
  return this->Result_;
}

int Market::open()
{
  // Tole bi moral prebrati iz parametrov
  db = new PgDatabase(DB_NAME);

  if ( db->ConnectionBad() ) {
    ACE_ERROR_RETURN((LM_ERROR, "Error %s connecting to database %s\n", 
		      db->ErrorMessage(), DB_NAME), -1); 
  }
}

/* Procedura, ki vse opravi: */

int Market::Add(Request &req)
{

  if (!req.IsValid())
    ACE_ERROR_RETURN((LM_ERROR, ...), -1);

  /* Dodaj */
  if (req.Save(db)==-1)
    ACE_ERROR_RETURN((LM_ERROR, ...), -1);

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
template class ACE_Singleton<Market, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Market, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

