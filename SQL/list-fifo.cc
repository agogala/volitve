/*
 * list-fifo.cc
 * Lists contents of fifo table.
 *
 */

#include <iostream.h>
#include <libpq++.h>
#include <stdlib.h>
extern "C" {
#include "postgres.h"		// for Postgres types
#include "utils/geo_decls.h" // for the POLYGON type
	   }

main()
{
  // Begin, by connecting to the backend using hardwired constants
  // and a test database created by the user prior to the invokation
  // of this test program.  Connect using cursor interface.
  //  char* dbName = getenv("VOLDB"); // change this to the name of your test database

  char* dbName = "volitve";

  PgDatabase data(dbName);

  // check to see that the backend connection was successfully made
  if ( data.ConnectionBad() ) {
    cerr << "Connection to database '" << dbName << "' failed." << endl
	 << data.ErrorMessage();
    exit(1);
  }

  
  // Print out the information about the extracted tuple
  char c;
  ExecStatusType status;

  char *statement;

  while (1) {
    cin >> c;

    statement = " ";

    switch (c) {
    case 'f': {
      statement = "select * from fifo";
      break;
    }
      
    case 's': {
      statement = "select * from stranke";
      break;
    }

    case 'k': {
      statement = "select * from papirji";
      break;
    }

    case 'p': {
      statement = "select * from posli";	
      break;
    }
    }

    if ((status = data.Exec(statement)) == PGRES_TUPLES_OK) {   
      /*      for (int i=0; i < data.Tuples(); i++) {
	cout << "tuple " << i << ": got" << endl;
	for (int j=0; j < data.Fields(); j++) {
	  cout << data.FieldName(j) << " = (" << 
	    data.GetLength(i, j) << " bytes) " << data.GetValue(i,j) << 
	    "," << endl;
	}
      }*/
      data.DisplayTuples();
    } else {
      cout << "No tuples returned..." << endl
	   << "status = " << status << endl
	   << "Error returned: " << data.ErrorMessage() << endl;
      break;
    }	
  }
}
