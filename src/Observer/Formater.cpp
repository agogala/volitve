/* -*- C++ -*-
 * $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
 *
 * $Id: Formater.cpp 1.1.2.3 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
 *
 * Oblikuje HTML datoteke.
 */

#include <stdio.h>
#include <Python.h>

#include "Config.h"
#include "Formater.h"

Formater::Formater(State *state, strset *userset)
{
  this->state_ = state;
  this->userset_ = userset;
 
  this->initialized_ = false;
}

int Formater::init()
{
  if (initialized_)
    return 0;

  ACE_DEBUG((LM_DEBUG, "Initializing python interpreter\n"));

  // Pripravi interpeter
  Py_Initialize();

  // Nalo¾i skripte:
  FILE *f;
  do {

    if ((f = fopen("./initfmt.py", "r"))==NULL) {
      ACE_ERROR((LM_ERROR, "Can't find init script\n"));
      break;
    }


    if (PyRun_SimpleFile(f, "Formater")!=0) {
      ACE_ERROR((LM_ERROR, "Error initializing formater\n"));
      break;
    }

    fclose(f);

    initialized_ = true;

  } while (0);

  return initialized_ ? 0 : -1;
}

int Formater::handle_timeout(const ACE_Time_Value &,
                                 const void *arg)
{
  if (!initialized_)
    init();

  if (state_->is_set()) {
    ACE_DEBUG((LM_DEBUG, "Formatting pregled.\n"));
    if (PyRun_SimpleString("MakePregled.run(admin_cfg.tempdir, admin_cfg.htmldir, admin_cfg.templates)")) {
      ACE_ERROR((LM_ERROR, "Error running MakePregled\n"));
    }
    state_->set(false);
  }
  while (userset_->size()>0) {
    char cmd[500];

    const char *ime = *(userset_->begin());
    ACE_OS::sprintf(cmd, "MakePregled.defupdateuser('%s')", ime);
    ACE_DEBUG((LM_DEBUG, "Formatting: %s\n", ime));
    if (PyRun_SimpleString(cmd)) {
      ACE_ERROR((LM_ERROR, "Error running MakePregled.defupdateuser\n"));
    }
    userset_->erase(userset_->begin());
    delete ime;
  }

  return 0;
}

/*
int Formater::mkpregled(void) {
  return PyRun_SimpleString("MakePregled()");
}
*/
/*
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Formater, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Formater, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

