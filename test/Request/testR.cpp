#include <ace/Singleton.h>
#include <ace/Log_Msg.h>
#include <libpq++.h>
#include <iostream.h>
#include "Config.h"
#include "Request.h"
#include "Stanje.h"

int
main()
{
  PgDatabase *db;
  
  db = new PgDatabase(DB_NAME);

  if ( db->ConnectionBad() ) {
    ACE_ERROR_RETURN((LM_ERROR, "Error %s connecting to database %s\n", 
		      db->ErrorMessage(), DB_NAME), -1); 
  }  

  //  Request req1("BUY KUCA 5 40.0 miha");

  cout << "Reading ..." << endl;

  cout << "Return: " << db->ExecTuplesOk("SELECT oid, * FROM fifo Where Ponudnik='miha'") << endl;

  Request req1;

  //  for (int i=0; i< db->Tuples(); i++) {
    req1.Read(*db, 0);
    //    cout << "Request: " << i << endl;
    //    cout << "   Valid: " << req.IsValid(*db) << endl;
    cout << "   Fields: " << req1.Ponudnik() << " " 
	 << req1.Papir_ID() << " " 
         << req1.Kolicina() << " " 
	 << req1.Cena() << endl;

    //  }


  //  cout << "Is Valid: " << req1.IsValid(*db) << endl;
  
  cout << "Kolicina: " << req1.Kolicina() << endl;

  req1.Omeji(*db);

  cout << "Omejitev: " << req1.Kolicina() << endl;


  /*  
  cout << "Request1" << endl;

  Request req1("BUY KUCA 10 60.1 andrej");
 
  req1.Store(*db);

  //  cout << "Request1: " << db->Tuples() << " status: " <<db->OidStatus() << endl;

  //  cout << "Index: " << db->FieldNum("blabla") << endl;

  cout << "Request2" << endl;

  Request req2("KUCA 10 60.1 Crt");

  req2.Store(*db);

  cout << "Request3" << endl;

  Request req3("SELL KUCA 200 102.3 MILAN");

  req3.Store(*db);

  cout << "Reading ..." << endl;

  cout << "Return: " << db->ExecTuplesOk("SELECT oid, * FROM zahtevki") << endl;

  Request req;

  for (int i=0; i< db->Tuples(); i++) {
    req.Read(*db, i);
    cout << "Request: " << i << endl;
    //    cout << "   Valid: " << req.IsValid(*db) << endl;
    cout << "   Fields: " << req.Ponudnik() << " " 
	 << req.Papir_ID() << " " 
         << req.Kolicina() << " " 
	 << req.Cena() << endl;
  }
  */
  delete db;

  return 0;

}

template class ACE_Singleton<ShrambaStanj, ACE_Null_Mutex>;
