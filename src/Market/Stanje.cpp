/* 
 * $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
 *
 * $Id: Stanje.cpp 1.2 Tue, 04 Nov 1997 18:56:32 +0000 andrej $
 *
 * Stanje stranke in hramba le teh.
 */

#include <ace/Log_Msg.h>
#include "Config.h"
#include "Stanje.h"
#include "Query.h"

inline int min(int a,int b) 
{
  return a<b ? a : b;
};

inline int max(int a,int b) 
{
  return a>b ? a : b;
};

Stanje::Stanje(const Stanje& s) : StanjeContent((StanjeContent)s)
{
  User_ID_ = new char[ strlen(s.User_ID_) + 1];
  strcpy(User_ID_, s.User_ID_);
}

Stanje& Stanje::operator=(const Stanje& s)
{
  StanjeContent::operator= ((StanjeContent) s);
  delete User_ID_;

  User_ID_ = new char[ strlen(s.User_ID_) + 1];
  strcpy(User_ID_, s.User_ID_);
  
}

// Stanje neke stranke. Vedno se prebere iz baze.
int
Stanje::Read(PgDatabase &db, const char *User_ID)
{
  delete User_ID_;
  User_ID_ = new char[strlen(User_ID)+1];
  strcpy(User_ID_, User_ID);

  // Preberi stanje
  // Paramerter
  // Stranka

  Query GetStanje=
    "SELECT * FROM Stanje WHERE Stranka_id='%s' ORDER BY Papir_ID";

  if (!db.Exec(GetStanje.Params(NULL, User_ID))) {
    ACE_ERROR_RETURN((LM_ERROR, "Error selecting Stanje: %s \n",
	       db.ErrorMessage()), -1);
  };
  
  int indPapir_ID = db.FieldNum("papir_id");
  int indKolicina = db.FieldNum("kolicina");

  char *buff;

  for (int i=0; i<db.Tuples(); i++) {
    buff = new char[MAX_PAPIR_ID+1];
    strcpy(buff, (char *)db.GetValue(i, indPapir_ID));
    rstrip(buff);
    erase(buff);
    double kol =  atof(db.GetValue(i, indKolicina));
    value_type val(buff, kol);
    insert(val);
  }

  return 0;
}

Interval 
Stanje::Dovoljeno(PgDatabase &db, const char* Papir_ID)
{
  // Za denar ne preverjamo!
  if (!strcmp(Papir_ID, denar_ID))
    return Interval(0,0);

  iterator i;

  i = begin();

  int S = 0;

  while (i!=end()) {
    if (strcmp((*i).first, denar_ID) &&
	strcmp((*i).first, Papir_ID)) 
      S += abs((int)(*i).second);
    i++;
  }

  int kp = (int)(*find(Papir_ID)).second;
  
  if (S + abs(kp)>MAX_KOL)
    return Interval(0,0);
  else
    return (Interval(S - MAX_KOL - kp, MAX_KOL - S - kp));
}

int
Stanje::Omeji(PgDatabase &db, const char* Papir_ID, int Kolicina)
{
  if (Kolicina<0)
    return max(Kolicina, Dovoljeno(db, Papir_ID).first);
  else
    return min(Kolicina, Dovoljeno(db, Papir_ID).second);
}

int 
Stanje::Spremeni(PgDatabase &db, const char* Papir_ID, 
	       int Kolicina, double Cena)
{
  // Za denar ne preverjamo!
  if (!strcmp(Papir_ID, denar_ID))
    ACE_ERROR_RETURN((LM_ERROR, "Can't change money!\n"), -1);
    
  // Popravi stanje:
  // Parametri:
  // (double) Kolicina
  // papir
  // stranka
  Query UpdateStanje =
    "UPDATE stanje\n"
    "SET kolicina=kolicina + %f\n"
    "WHERE papir_id='%s' AND\n"
    "      stranka_id='%s'";

  // Papir:
#if defined(DEBUG)
  cout << UpdateStanje.Params
			  (NULL, 
			   (double)Kolicina,
			   Papir_ID,
			   User_ID_) << endl;
#endif

  if ((!db.ExecCommandOk(UpdateStanje.Params
			  (NULL, 
			   (double)Kolicina,
			   Papir_ID,
			   User_ID_)))) {
    ACE_ERROR_RETURN((LM_ERROR, "Error updating stanje: %s\n", 
		     db.ErrorMessage()), -1);
  }

  // Denar:
#if defined(DEBUG)
  cout << UpdateStanje.Params
			  (NULL, 
			   -(double)Kolicina * Cena,
			   denar_ID, 
			   User_ID_) << endl;
#endif

  if ((!db.ExecCommandOk(UpdateStanje.Params
			  (NULL, 
			   -(double)Kolicina * Cena,
			   denar_ID, 
			   User_ID_)))) {
    ACE_ERROR_RETURN((LM_ERROR, "Error updating stanje: %s\n", 
		      db.ErrorMessage()), -1);
  }

  // Popravimo ¹e stanje v spominu:
  (*this)[Papir_ID] += (double)Kolicina;
  (*this)[denar_ID] += -(double)Kolicina * Cena;

  return 0;
}

char *Stanje::User_ID()
{
  return this->User_ID_;
}


ShrambaStanj::iterator 
ShrambaStanj::Get(PgDatabase &db, const char* User_ID)
{
  iterator i;

#if defined(DEBUG)
  cout << "Shramba: " << User_ID << endl;
#endif

  if ((i=find(User_ID)) != end()) 
    if (!strcmp(((*i).second)->User_ID(), User_ID)) {
#if defined(DEBUG)
      cout << "** V shrambi" << endl;
#endif
      return i;
    }
#if defined(DEBUG)
  cout << "** Nov" << endl;
#endif

  Stanje *s = new Stanje;
  if (s->Read(db, User_ID)<0)
    return end();
  
  return insert(value_type(User_ID, s)).first;
}

Stanje dummyStanje;
ShrambaStanj dummyShramabStanj;
Interval dummyInterval;

/*void DummyInit()
{

StanjeMap dummySM;

StanjeCache dummySC;

Interval dummyInterval;

dummySM["a"] = 1.0;
 
}
*/
