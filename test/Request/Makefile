#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for the Reactor version of the Market Deamon
# 	Heavily borrowed from Logging Server Deamon
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= testR

FILES	= Request \
	  Query

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

INCLDIRS += -I$(PG_ROOT)/include -I../../src/Market

LDLIBS += -lpq++ -lpq -lg++ -lbb

LDFLAGS += -L$(PG_ROOT)/lib -R$(ACE_ROOT)/ace -L../../src/lib

INCLDIRS += -I../../src/include

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.bin.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU

#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Request.o .shobj/Request.so: Request.cpp ../../src/Market/Request.h \
 ../../src/include/Config.h ../../src/Market/Query.h \
 ../../src/Market/MarketErrors.h
.obj/Query.o .shobj/Query.so: Query.cpp ../../src/Market/Query.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
