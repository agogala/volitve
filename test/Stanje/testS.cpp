#include <libpq++.h>
#include <iostream.h>
#include "Stanje.h"

void
printStanje(Stanje &s)
{
  cout << endl;
  cout << "Stanje za: " << s.User_ID() << endl;
  
  Stanje::iterator i;

  for (i = s.begin(); i!=s.end(); i++) {
    cout << (*i).first << ": " << (*i).second << endl;
  }  
}

void 
printOmejitve(PgDatabase &db, Stanje &s)
{
  cout << endl;
  cout << "Omejitve za: " << s.User_ID() << endl;

  Stanje::iterator i;
  Interval minmax;

  for (i = s.begin(); i!=s.end(); i++) {
    minmax = s.Dovoljeno(db, (*i).first);

    cout << (*i).first << ": " << minmax.first << " - " 
	 << minmax.second << endl;
	 
  }

}

void printOmejitev(PgDatabase &db, Stanje &s, char *Papir)
{
  Interval minmax;

  minmax = s.Dovoljeno(db, Papir);

  cout << minmax.first << " - " << minmax.second << endl;
}


int
main(int argc, char** argv)
{
  PgDatabase *db;

  if (argc<3) {
    cout << "Usage: " << argv[0] << " <user> <papir>" << endl;
    return -1;
  }
    
  char *Ponudnik = argv[1];
  char *Papir = argv[2];

  db = new PgDatabase("zupani");

  Stanje *s = new Stanje();
  s->Read(*db, Ponudnik);

  //  ShrambaStanj stanja;

  //  printStanje(*s);

  //  printOmejitve(*db, *s);

  printOmejitev(*db, *s, Papir);
    
  delete db;

  return 0;
}
