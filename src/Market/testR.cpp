#include "Request.h"

int
main()
{
  Request req;

  req.open();

  req.Add("BUY KUCA 10 60.1 Crt");

  req.Add("KUCA 10 60.1 Crt");

  req.Add("SELL KUCA 200 102.3 MILAN");
}
