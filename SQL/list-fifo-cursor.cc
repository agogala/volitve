/*
 * list-fifo.cc
 * Lists contents of fifo table.
 * Uses cursors - blocks adding new records...
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

  PgCursor data(dbName, "mycursor");

  // check to see that the backend connection was successfully made
  if ( data.ConnectionBad() ) {
    cerr << "Connection to database '" << dbName << "' failed." << endl
         << data.ErrorMessage();
    exit(1);
  }

  // Declare a binary cursor for all the tuples in database 'test1'
  if ( !data.Declare("select * from fifo", 0) ) {
    cerr << "DECLARE CURSOR command failed" << endl;
    exit(1);
  }

  // fetch all instances from the current cursor
  if ( !data.Fetch() ) {
    cerr << "FETCH ALL command didn't return tuples properly" << endl;
    exit(1);
  }
 
  /*
  for (int i=0;i<data.Fields();i++) {
      printf("type[%d] = %d, size[%d] = %d\n",
	     i, data.FieldType(i), 
	     i, data.FieldSize(i));
  }
  */
  
  // Print out the information about the extracted tuple
  char c;
  while (1) {
    cin >> c;
    for (int i=0; i < data.Tuples(); i++) {
      cout << "tuple " << i << ": got" << endl;
      for (int j=0; j < data.Fields(); j++) {
	cout << data.FieldName(j) << " = (" << 
	  data.GetLength(i, j) << " bytes) " << data.GetValue(i,j) << 
	  "," << endl;
      }
    }
  }
}
