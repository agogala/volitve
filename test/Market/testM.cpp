#include <ace/Log_Msg.h>
#include <libpq++.h>
#include <iostream.h>
#include "Config.h"
#include "Request.h"
#include "Market.h"

int
main()
{

  if (MARKET::instance ()->open()==-1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "Error opening database connection\n"), -1);

  cout << "Request1" << endl;

  Request req1("SELL KUCA 10 60.1 Crt");
 
  cout << "  " << MARKET::instance ()->Add(req1) ;

  cout << " " << MARKET::instance ()->Result() << endl;

  cout << "Request2" << endl;

  Request req2("BUY KUCA 10 60.1 Crt");

  cout << "  " << MARKET::instance ()->Add(req2) ;

  cout << " " << MARKET::instance ()->Result() << endl;

  /*
  cout << "Request3" << endl;

  Request req3("SELL KUCA 200 102.3 MILAN");

  */
  return 0;

}
