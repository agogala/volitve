/* -*- C++ -*-
 * $ProjectHeader: volitve 0.13 Wed, 24 Sep 1997 19:03:46 +0200 andrej $
 *
 * $Id: Notification_Handler.h 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
 *
 * Zaznava spremembe na trgu.
 */

#if !defined(NOTIFICATION_HANDLER_H)
#define NOTIFICATION_HANDLER_H

#include <ace/Event_Handler.h>
#include <ace/SOCK_Dgram.h>
#include <ace/INET_Addr.h>

#include "State.h"

class Notification_Handler : public ACE_Event_Handler, public ACE_SOCK_Dgram
{
  // = TITLE
  //     Receive broadcast messages from market.
  //
  // = DESCRIPTION
  //     This class waits for broadcast messages from market and sets 
  //     appropriate flags.
public:
  Notification_Handler (const ACE_INET_Addr &local_addr, State *state);
 
  virtual ACE_HANDLE get_handle (void) const;
 
  virtual int handle_input (ACE_HANDLE handle);

private:
  State *state_;
};

#endif // NOTIFICATION_HANDLER_H
