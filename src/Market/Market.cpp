/*
 * $ProjectHeader: volitve 0.7 Mon, 08 Sep 1997 17:37:41 +0200 andrej $
 *
 * $Id: Market.cpp 1.5 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
 *
 * Trg. Zna dodajati zahtevke.
 */

#include <ace/OS.h>
//#include <stdlib.h>

#include "VectorRequest.h"
#include "Market.h"
#include "Request.h"
#include "Query.h"
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

  return 0;
}

/* Procedura, ki vse opravi: */

#define MARKET_ERROR(X,Y) \
     LastRC_ = Y; \
     strcpy(this->Result_, X)

int Market::Add(Request &req)
{
  // oznaka, ki pove èe je pri¹lo do napake.
  bool error = false;

  if (db==NULL) {
    MARKET_ERROR("Internal error\n", -1);
    ACE_ERROR_RETURN((LM_ERROR, "Not connected\n"), -1);
  }

  if (!req.IsValid(*db)) {
    MARKET_ERROR("Request not valid\n", -1);
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid\n"), -1);
  }

  do {
    // Prièni s transakcijo. 
    db->Exec("BEGIN");

    // Dodaj 
    if (error = (req.Store(*db)==-1)) {
      ACE_ERROR((LM_ERROR, "Can't store request!\n"));
      break;
    }

    // Poi¹èemo vse kandidate: 
    // Parametri:
    //    Vrsta naroèila (N ali P)
    //    Enakost (<= ali =>)
    //    Cena
    //    Papir_ID
    //    Vrsta sortiranja

    Query MatchFIFO = 
      "SELECT oid, *\n"
      "FROM fifo\n"
      "WHERE\n"
      "     kolicina %s 0 and\n"
      "     cena %s %f and\n"
      "     papir_id = '%s'\n"
      "ORDER BY\n"
      "     cena using %s, datum, ura";
  
    char *KolOp = req.Kolicina()<0 ? ">" : "<";

    char *CenaOp = req.Kolicina()<0 ? "<=" : ">=";

    char *Urejenost = req.Kolicina()<0 ? "<" : ">";

    if (error = !db->Exec(MatchFIFO.Params
		  (NULL, KolOp, CenaOp, req.Cena(), 
		   req.Papir_ID(), Urejenost))) {
      ACE_ERROR((LM_ERROR, "Error selecting FIFO: %s \n",
			db->ErrorMessage()));
      break;
    }

    //  db->DisplayTuples();
  
    // Preberemo vse ustrezne posle iz vrste 
    VectorRequest vecreq(db->Tuples());
    Request freq;
  
    for (int i=0; i<db->Tuples(); i++) {
      freq.Read(*db, i);
      // Veljavnosti ne preverjamo, ker imamo samo eno povezavo z bazo... hm hm
      //    if (!freq.IsValid()) {
      //      MARKET_ERROR("Internal error\n", -1);
      //      ACE_ERROR_RETURN((LM_ERROR, "FIFO containes invalid request!\n"), -1);
      //    }
      vecreq.push_back(freq);
    }  

    // Vstavi v sklenjene posle.
    // Parametri:
    // Papir_ID
    // Cena
    // Kolicina
    // Kupec
    // Prodajalce
    Query InsertPosli = 
      "INSERT INTO posli\n"
      "VALUES('today', 'now', '%s', %f, %d, '%s', '%s')";
  
  // Zbri¹i iz FIFO:
  // Parametri:
  // oid
    Query DeleteFIFO = "DELETE FROM fifo WHERE oid=%s";

    // Popravi kolièino v FIFO
    // Parametra:
    // nova kolièina:
    // oid
    Query UpdateFIFO = "UPDATE fifo SET kolicina=%d WHERE oid=%s";

  // ©tevilo ¹e ne zaprtih poslov
    int Odprti = req.Kolicina();

    {

      int n = vecreq.size();
    
  
      for (int i=0; i<n && Odprti!=0; i++) {
	// Sklepaj posle:
	int Kolicina;

	// Tole moramo doloèiti sedaj, dokler je Odprti!=0!
	Request *buy = (Odprti<0 ? &req : &vecreq[i]);
	Request *sell = (Odprti>0 ? &req : &vecreq[i]);

	// Popravi FIFO:
	if (abs(Odprti)<abs(vecreq[i].Kolicina())) {
	  // Tale posel je dovolj dober. 
	  Kolicina = abs(Odprti);

	  /*	ACE_DEBUG((LM_DEBUG, "Kol: %d, ID: %s, vs: %d\n", 
		vecreq[i].Kolicina(), vecreq[i].ID(), 
		vecreq[i].Kolicina() + Odprti));*/

	  if (error = (!db->ExecCommandOk(UpdateFIFO.Params
				 (NULL, 
				  vecreq[i].Kolicina() + Odprti,
				  vecreq[i].ID())))) {
	    ACE_ERROR((LM_ERROR, "Error updating FIFO: %s\n", 
		       db->ErrorMessage()));
	    break;
	  }

	  Odprti = 0;

	} else {
	  Kolicina = abs(vecreq[i].Kolicina());
	  Odprti += vecreq[i].Kolicina();
	
	  if (error = (!db->ExecCommandOk(DeleteFIFO.Params
				 (NULL, vecreq[i].ID())))) {
	    ACE_ERROR((LM_ERROR, "Error deleting from FIFO: %s\n", 
		       db->ErrorMessage()));
	    break;
	  }
	}

	// Dodaj posel:
	if (error = (!db->ExecCommandOk(InsertPosli.Params
			       (NULL, buy->Papir_ID(), 
				vecreq[i].Cena(), 
			        Kolicina,
				buy->Ponudnik(), sell->Ponudnik())))) {
	  ACE_ERROR((LM_ERROR, "Error inserting into posli: %s\n", 
		     db->ErrorMessage()));
	  break;
	}
	// Popravi stanje [...]      
      } 

      // Vstavi posel v FIFO:
      // Parametri:
      //  Kolièina
      //  oid zahtevka.

      Query InsertFIFO = 
	"INSERT INTO FIFO\n"
	"SELECT cena, %d AS kolicina,\n"
	"       papir_id, datum, ura, ponudnik\n"
	"FROM zahtevki\n"
	"WHERE oid=%s";

      if ((!error) && (Odprti!=0)) {
	if (error = (!db->ExecCommandOk(InsertFIFO.Params
			       (NULL, Odprti, req.ID())))) {
	  ACE_ERROR((LM_ERROR, "Error inserting into FIFO: %s\n", 
		     db->ErrorMessage()));
	}
      }
      
    } while (0);
    
    if (!error) {
      db->Exec("END");
    } else {
      db->Exec("ROLLBACK");
      LastRC_ = -1;
      strcpy(Result_, "Internal error\n");
      ACE_ERROR_RETURN((LM_ERROR, "Internal error\n"), -1);
    }
  } while (0);

  strcpy(this->Result_, "OK");
  LastRC_ = 0;
  return LastRC_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Market, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Market, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

