/* -*- C++ -*-
 * $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
 *
 * $Id: Registrator.cpp 1.3 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
 *
 * Oblikuje HTML datoteke.
 */

#include <stdio.h>
#include <Python.h>
#include <ace/OS.h>
// Python konstante za run:
#include <graminit.h>

#include "Config.h"
#include "Registrator.h"

// ----------------------------
/*
RegRec::RegRec(const char *hash, 
	 const char *username,
	 const char *passwd)
{
  hash_ = new char[ACE_OS::strlen(hash)+1];
  ACE_OS::strcpy(hash_, hash);

  username_ = new char[ACE_OS::strlen(username)+1];
  ACE_OS::strcpy(username_, username);

  passwd_ = new char[ACE_OS::strlen(passwd)+1];
  ACE_OS::strcpy(passwd_, passwd);
}

RegRec::~RegRec()
{
  delete passwd_;
  delete username_;
  delete hash_;
}

char *RegRec::username()
{
  return this->username_;
}

char *RegRec::passwd()
{
  return this->passwd_;
}
 
char *RegRec::hash()
{
  return this->hash_;
}
*/
// ----------------------------

Registrator::Registrator()
{
    this->initialized_ = false;
}

int Registrator::init()
{
  if (initialized_)
    return 0;

  ACE_DEBUG((LM_DEBUG, "Initializing python interpreter\n"));

  // Pripravi interpeter
  Py_Initialize();

  // Nalo¾i skripte:
  FILE *f;
  do {

    if ((f = fopen("./initadm.py", "r"))==NULL) {
      ACE_ERROR((LM_ERROR, "Can't find init script\n"));
      break;
    }

    if (PyRun_SimpleFile(f, "Registrator")!=0) {
      ACE_ERROR((LM_ERROR, "Error initializing formater\n"));
      break;
    }

    fclose(f);

    initialized_ = true;

  } while (0);

  return initialized_ ? 0 : -1;
}

int Registrator::Validate(const char *hash)
{

  if (hash==NULL)
    return 200; // Syntax error

  if (!initialized_)
    init();

  static char CmdLine[] = "Registrator.Validate('%s')";

  char buff[strlen(CmdLine) + strlen(hash) + 1];

  ACE_OS::sprintf(buff, CmdLine, hash);

  return PythonRunInt(buff);
}

int Registrator::Register(char *hash, char *username, char *passwd)
{
  if ((hash==NULL) || (username==NULL) || (passwd==NULL))
    return 200; // Syntax error

  if (!initialized_)
    init();

  static char CmdLine[] = "Registrator.Registriraj('%s','%s','%s')";

  char buff[strlen(CmdLine) + strlen(hash) +
	   strlen(username) + strlen(passwd) + 1];

  ACE_OS::sprintf(buff, CmdLine, hash, username, passwd);

  return PythonRunInt(buff);

}

int Registrator::UserID(const char *user, char *ID)
{
  if ((user==NULL) || (ID==NULL))
    return 200;

  if (!initialized_)
    init();

  static char CmdLine[] = "Registrator.UserID('%s')";

  char buff[strlen(CmdLine) + strlen(user) + 1];

  ACE_OS::sprintf(buff, CmdLine, user);

  return PythonRunStr(buff, ID);
}

int Registrator::PythonRunInt(char *Command)
{
  PyObject *m, *d;
  PyIntObject *v;

  ACE_DEBUG((LM_DEBUG, "(int) Calling python with: %s\n", Command));

  m =  PyImport_AddModule("__main__");
  if (m == NULL)
    ACE_ERROR_RETURN((LM_ERROR, "Error importing main module"), 500);
  d = PyModule_GetDict(m);

  v = (PyIntObject *)PyRun_String(Command, eval_input, d, d);

  if (v == NULL) {
    PyErr_Print();
    return 500;
  }

  int rc = PyInt_AS_LONG(v);
  
  Py_DECREF(v);
  Py_FlushLine();  

  ACE_DEBUG((LM_DEBUG,"Python run rc: %d\n", rc));

  return rc;
}

// Poklièi Python, kjer funkcija vrne urejen par int, str
int Registrator::PythonRunStr(char *Command, char *Result)
{
  PyObject *m, *d;
  PyObject *v;

  char *outs;
  int rc;

  ACE_DEBUG((LM_DEBUG, "(str) Calling python with: %s\n", Command));

  m =  PyImport_AddModule("__main__");
  if (m == NULL) {
    ACE_ERROR((LM_ERROR, "Error importing main module"));
    return 500;
  }
  d = PyModule_GetDict(m);

  v = PyRun_String(Command, eval_input, d, d);

  if (v == NULL) {
    PyErr_Print();
    return 500; // Internal error
  }

  PyArg_ParseTuple(v, "is", &rc, &outs);

  strcpy(Result, outs);
  
  Py_DECREF(v);
  Py_FlushLine();  

  return rc;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Registrator, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Registrator, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

