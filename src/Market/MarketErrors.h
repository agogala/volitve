/* -*- C++ -*-
 * $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
 * 
 * $Id: MarketErrors.h 1.2 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
 *
 * Konstante, ki definirajo napake v programu.
 */

#if !defined(MARKET_ERRORS_H)
#define MARKET_ERRORS_H

#define me_OK            000
#define me_NoSuchSymbol  100
#define me_WrongNoShares 101
#define me_WrongPrice    102
#define me_NoSuchUser    103
#define me_SyntaxError   200
#define me_InternalError 500
// Preklic:
#define me_NoSuchRequest 100

#endif // MARKET_ERRORS_H
