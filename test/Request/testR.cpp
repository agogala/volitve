#include <ace/Log_Msg.h>
#include <libpq++.h>
#include "Config.h"
#include "Request.h"

int
main()
{
  PgDatabase *db;
  
  db = new PgDatabase(DB_NAME);

  if ( db->ConnectionBad() ) {
    ACE_ERROR_RETURN((LM_ERROR, "Error %s connecting to database %s\n", 
		      db->ErrorMessage(), DB_NAME), -1); 
  }  

  Request req1("BUY KUCA 10 60.1 Crt");
 
  req1.Store(*db);

  Request req2("KUCA 10 60.1 Crt");

  req2.Store(*db);

  Request req3("SELL KUCA 200 102.3 MILAN");

  req3.Store(*db);

  delete db;

  return 0;

}
