/*
 * $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
 *
 * $Id: Market.cpp 1.13 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
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

int AddLog(PgDatabase &db, Request &req, char* Razlog, ...)
{
  /*  char buff[256];

  va_list ap;
  va_start(ap, first);

  vsprintf(buff, Razlog, ap);

  va_end(ap);

  //  [...]
  */
  return 0;
}

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

int Market::Add(Request &req, strset *userset = NULL)
{
  // oznaka, ki pove �e je pri�lo do napake.
  bool error = false;

  if (db==NULL) {
    MARKET_ERROR("Internal error\n", 500);
    ACE_ERROR_RETURN((LM_ERROR, "Not connected\n"), 500);
  }

  if (!req.IsValid(*db)) {
    MARKET_ERROR("Request not valid\n", req.LastError());
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid\n"), req.LastError());
  }

  // Pri�ni s transakcijo. 
  db->Exec("BEGIN");

  do {    
    // Dodaj 
    if (error = (req.Store(*db)!=0)) {
      ACE_ERROR((LM_ERROR, "Can't store request!\n"));
      break;
    }

    // Poi��emo vse kandidate: 
    // Parametri:
    //    Vrsta naro�ila (N ali P)
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

    char *CenaOp = req.Kolicina()<0 ? ">=" : "<=";

    char *Urejenost = req.Kolicina()<0 ? ">" : "<";

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
    //    Request freq;
  
    for (int i=0; i<db->Tuples(); i++) {
      //      freq.Read(*db, i);
      // Veljavnosti ne preverjamo, ker imamo samo eno povezavo z bazo... hm hm
      //    if (!freq.IsValid()) {
      //      MARKET_ERROR("Internal error\n", -1);
      //      ACE_ERROR_RETURN((LM_ERROR, "FIFO containes invalid request!\n"), -1);
      //    }
      //      vecreq.push_back(freq);
      vecreq.push_back(new Request(*db, i));
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
  
    // Zbri�i iz FIFO:
    // Parametri:
    // oid
    Query DeleteFIFO = "DELETE FROM fifo WHERE oid=%s";

    // Popravi koli�ino v FIFO
    // Parametra:
    // nova koli�ina:
    // oid
    Query UpdateFIFO = "UPDATE fifo SET kolicina=%d WHERE oid=%s";

    // AddLog()...
    int ReqKolicina = req.Kolicina();

    ACE_DEBUG((LM_DEBUG, "Koli�ina: %d\n", ReqKolicina));

    req.Omeji(*db);

    ACE_DEBUG((LM_DEBUG, "Omejena koli�ina: %d\n", req.Kolicina()));
      
    // Ime stranke:
    char *ime;
        
    int n = vecreq.size();
    
    for (int i=0; i<n && req.Kolicina()!=0; i++) {
      // Sklepaj posle:
      int Kolicina;
      
      // Tole moramo dolo�iti sedaj, dokler je req.Kolicina()!=0!
      Request *buy = (req.Kolicina()>0 ? &req : vecreq[i]);
      Request *sell = (req.Kolicina()<0 ? &req : vecreq[i]);

      // AddLog()...
      int VecReqKolicina = vecreq[i]->Kolicina();
      vecreq[i]->Omeji(*db);
      
      // Popravi FIFO:
      if (abs(req.Kolicina())<abs(vecreq[i]->Kolicina())) {
	// Tale posel je dovolj dober. 
	Kolicina = abs(req.Kolicina());
	
	if (error = (!db->ExecCommandOk(UpdateFIFO.Params
					(NULL, 
					 VecReqKolicina + req.Kolicina(),
					 vecreq[i]->ID())))) {
	  ACE_ERROR((LM_ERROR, "Error updating FIFO: %s\n", 
		     db->ErrorMessage()));
	  break;
	}
	
	req.Spremeni(0);
	ACE_DEBUG((LM_DEBUG, "Celo naro�ilo\n"));

      } else {
	Kolicina = abs(vecreq[i]->Kolicina());
	req.Spremeni(req.Kolicina() + vecreq[i]->Kolicina());
	ReqKolicina += vecreq[i]->Kolicina();

	ACE_DEBUG((LM_DEBUG, "Del naro�ila, koli�ina: %d\n",ReqKolicina));
	ACE_DEBUG((LM_DEBUG, "Omejena koli�ina: %d\n",req.Kolicina()));

	if (error = (!db->ExecCommandOk(DeleteFIFO.Params
					(NULL, vecreq[i]->ID())))) {
	  ACE_ERROR((LM_ERROR, "Error deleting from FIFO: %s\n", 
		     db->ErrorMessage()));
	  break;
	}
      }

      if (Kolicina != 0) {
	// Preveri, �e se kupec in prodajalec razlikujeta:
	if (!ACE_OS::strcmp(buy->Ponudnik(), sell->Ponudnik())) {
	  // Posla ne sklenemo, shranimo opombo + dodamo uporabnika v spremembe
	  // AddLog()...

	  // Shrani ime v mno�ico:
	  if (userset!=NULL) {
	    ime = new char[strlen(buy->Ponudnik()) + 1];
	    ACE_OS::strcpy(ime, buy->Ponudnik());
	    if (!userset->insert(ime).second)
	      delete ime;
	  }
	  
	} else {
	  // Dodaj posel:
	  if (error = (!db->ExecCommandOk
		       (InsertPosli.Params
			(NULL, buy->Papir_ID(), 
			 vecreq[i]->Cena(), 
			 Kolicina,
			 buy->Ponudnik(), sell->Ponudnik())))) {
	    ACE_ERROR((LM_ERROR, "Error inserting into posli: %s\n", 
		       db->ErrorMessage()));
	    break;
	  }

	  Stanje *st;
	  st = (*(STANJA::instance() ->Get(*db, buy->Ponudnik()))).second;
  
	  if (error = 
	      (st->Spremeni(*db, buy->Papir_ID(), Kolicina, 
			    vecreq[i]->Cena())<0)) {
	    ACE_ERROR((LM_ERROR, "Error changing buyer's stanje: %s\n",
		       db->ErrorMessage()));
	    break;
	  }

	  st = (*(STANJA::instance() ->Get(*db, sell->Ponudnik()))).second;
	  if (error = 
	      (st->Spremeni(*db, sell->Papir_ID(), -Kolicina, 
			    vecreq[i]->Cena())<0)) {
	    ACE_ERROR((LM_ERROR, "Error changing seller's stanje: %s\n",
		       db->ErrorMessage()));
	    break;
	  }

	  // Shrani ime v mno�ico:
	  if (userset!=NULL) {
	    // Kupec:
	    ime = new char[strlen(buy->Ponudnik()) + 1];
	    ACE_OS::strcpy(ime, buy->Ponudnik());
	    if (!userset->insert(ime).second)
	      delete ime;
	  
	    // Prodajalec:
	    ime = new char[strlen(sell->Ponudnik()) + 1];
	    ACE_OS::strcpy(ime, sell->Ponudnik());
	    if (!userset->insert(ime).second)
	      delete ime;
	  }
	}
      } // Kolicina != 0
    } // for


    // Vstavi kar je ostalo od zahtevka v FIFO:
    // Parametri:
    //  Koli�ina
    //  oid zahtevka.

    Query InsertFIFO = 
      "INSERT INTO FIFO\n"
      "SELECT cena, %d AS kolicina,\n"
      "       papir_id, datum, ura, ponudnik\n"
      "FROM zahtevki\n"
      "WHERE oid=%s";

    ACE_DEBUG((LM_DEBUG, "========\n"));
    ACE_DEBUG((LM_DEBUG, "Koli�ina: %d\n", ReqKolicina));
    ACE_DEBUG((LM_DEBUG, "Omejena koli�ina: %d\n", req.Kolicina()));


    if ((!error) && (req.Kolicina()!=0)) {
      if (error = (!db->ExecCommandOk(InsertFIFO.Params
				      (NULL, ReqKolicina, req.ID())))) {
	ACE_ERROR((LM_ERROR, "Error inserting into FIFO: %s\n", 
		   db->ErrorMessage()));
      } 
      else if (userset!=NULL) {
	ime = new char[strlen(req.Ponudnik()) + 1];
	ACE_OS::strcpy(ime, req.Ponudnik());
	if (!userset->insert(ime).second)
	  delete ime;
      }	
    }
    
    // Tale if je tu zato, �e bom kdaj kaj dodal za tem stavkom,
    // pa se ne bom spomnil, da moram ven...
    if (error = FixFIFO(KolOp, req.Papir_ID(), Urejenost, userset))
      break;

  } while (0);
    
  if (!error) {
    db->Exec("END");
  } else {
    db->Exec("ROLLBACK");
    LastRC_ = 500;
    strcpy(Result_, "Internal error\n");
    ACE_ERROR_RETURN((LM_ERROR, "Internal error\n"), 500);
  }

  strcpy(this->Result_, "OK");
  LastRC_ = 0;
  return LastRC_;
}


int Market::Cancel(Request &req, strset *userset = NULL)
{
  // oznaka, ki pove �e je pri�lo do napake.
  bool error = false;

  if (db==NULL) {
    MARKET_ERROR("Internal error\n", 500);
    ACE_ERROR_RETURN((LM_ERROR, "Not connected\n"), 500);
  }

  if (!req.IsValid(*db)) {
    MARKET_ERROR("Request not valid\n", req.LastError());
    ACE_ERROR_RETURN((LM_ERROR, "Request not valid\n"), req.LastError());
  }

  // Pri�ni s transakcijo. 
  db->Exec("BEGIN");

  do {    
    // Dodaj 
    if (error = (req.Store(*db)!=0)) {
      ACE_ERROR((LM_ERROR, "Can't store request!\n"));
      break;
    }

    // Zapomni si koli�ino in papir:
    // Parameter:
    // oid    
    Query GetFIFO=
      "SELECT oid, * FROM fifo\n"
      "WHERE oid=%s";

    if (error = (!db->Exec(GetFIFO.Params
			   (NULL, req.ID())))) {
      ACE_ERROR((LM_ERROR, "Error selecting FIFO: %s\n", 
		 db->ErrorMessage()));
      break;
    }

    if (error = (db->Tuples()==0)) {
      ACE_ERROR((LM_ERROR, "No request in FIFO!\n"));
      break;
    }
    
    // Preberemo zahtevek:
    Request freq(*db, 0);

    // Preverjamo �e vedno, le ukrepamo ne. �e je zahtevek neveljaven, toliko
    // bolje da ga zbri�emo!
    if (/*error =*/ !freq.IsValid(*db)) {
      ACE_ERROR((LM_ERROR, "FIFO Request not valid: %d\n",
		 freq.LastError()));
      //      break;
    }

    // Zbri�i iz FIFO:
    // Parameter:
    // oid
    Query DeleteFIFO=
      "DELETE FROM fifo\n"
      "WHERE oid=%s";

    if (error = (!db->ExecCommandOk(DeleteFIFO.Params
				    (NULL, req.ID())))) {
      ACE_ERROR((LM_ERROR, "Error deleting from FIFO: %s\n", 
		 db->ErrorMessage()));
      break;
    }

    // AddLog()...

    // Shranimo ime spremembe:
    if (userset!=NULL) {
      char *ime = new char[strlen(req.Ponudnik()) + 1];
      ACE_OS::strcpy(ime, req.Ponudnik());
      if (!userset->insert(ime).second)
	delete ime;
    }		    

    // Popravimo FIFO:
    char *KolOp = freq.Kolicina()<0 ? ">" : "<";

    char *Urejenost = freq.Kolicina()<0 ? ">" : "<";
    
    if (error = FixFIFO(KolOp, freq.Papir_ID(), Urejenost, userset))
      break;

  } while (0);
    
  if (!error) {
    db->Exec("END");
  } else {
    db->Exec("ROLLBACK");
    LastRC_ = 500;
    strcpy(Result_, "Internal error\n");
    ACE_ERROR_RETURN((LM_ERROR, "Internal error\n"), 500);
  }

  strcpy(this->Result_, "OK");
  LastRC_ = 0;
  return LastRC_;


}

bool Market::FixFIFO(char *KolOp, const char *Papir_ID, 
		     char *Urejenost, strset *userset = NULL)
{
  bool error = false;

  // Popravi FIFO:
  // Parametri: (enake vrednosti kot pri MatchFIFO)
  //    Vrsta naro�ila (N ali P)
  //    Papir_ID
  //    Vrsta sortiranja

  Query GetFIFO =
    "SELECT oid, *\n"
    "FROM fifo\n"
    "WHERE\n"
    "     kolicina %s 0 and\n"
    "     papir_id = '%s'\n"
    "ORDER BY\n"
    "     cena using %s, datum, ura";

    // Query poganjam vedno znova, kar verjetno ni najbolje.
    // Mogo�e se vseeno spla�a najprej prebrati cel FIFO?
  do {    
    if (error = (!db->Exec(GetFIFO.Params
			   (NULL, KolOp, Papir_ID,
			    Urejenost)))) {
      ACE_ERROR((LM_ERROR, "Error selecting FIFO2: %s\n",
		 db->ErrorMessage()));
      break; // Sko�i ven iz te zanke, ne iz while(0)!
    }

    if (db->Tuples()==0)
      break;

    Request freq(*db, 0);

    // AddLog()...      
    freq.Omeji(*db);

    // Zbri�i iz FIFO:
    // Parametri:
    // oid
    Query DeleteFIFO = "DELETE FROM fifo WHERE oid=%s";

    if (freq.Kolicina()==0) {
      if (error = (!db->ExecCommandOk(DeleteFIFO.Params
				      (NULL, freq.ID())))) {
	ACE_ERROR((LM_ERROR, "Error deleting from FIFO2: %s\n", 
		   db->ErrorMessage()));
	break;
      }	
      // Shranimo ime spremembe:
      if (userset!=NULL) {
	char *ime = new char[strlen(freq.Ponudnik()) + 1];
	ACE_OS::strcpy(ime, freq.Ponudnik());
	if (!userset->insert(ime).second)
	  delete ime;
      }		
    } else
      break;      
  } while(1);

  return error;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Market, ACE_Null_Mutex>;
template class ACE_Singleton<ShrambaStanj, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Market, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<Stanje, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

