#----------------------------------------------------------------------------
# $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
#
# $Id: Makefile 1.8 Tue, 09 Sep 1997 22:58:50 +0000 andrej $
#
# Makefile za market deamon.
# 
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= marketd

FILES 	= Market_Handler \
	  Market \
	  Request \
	  VectorRequest \
	  Notifier \
	  Query

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

INCLDIRS += -I$(PG_ROOT)/include

LDLIBS += -lpq++ -lpq -lg++

LDFLAGS += -L$(PG_ROOT)/lib 

# Tole potrebujemo samo zaradi debugiranja na Solarisu.
ifeq ($(OS),SunOS)
LDFLAGS += -R$(ACE_ROOT)/ace
endif

INCLDIRS += -I../include

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

.obj/Market_Handler.o .shobj/Market_Handler.so: Market_Handler.cpp marketd.h Market.h Request.h \
 ../include/Config.h Market_Handler.h Notifier.h
.obj/Market.o .shobj/Market.so: Market.cpp VectorRequest.h Request.h ../include/Config.h \
 Market.h Query.h
.obj/Request.o .shobj/Request.so: Request.cpp Request.h ../include/Config.h Query.h
.obj/VectorRequest.o .shobj/VectorRequest.so: VectorRequest.cpp VectorRequest.h Request.h \
 ../include/Config.h
.obj/Notifier.o .shobj/Notifier.so: Notifier.cpp marketd.h Notifier.h ../include/Config.h
.obj/Query.o .shobj/Query.so: Query.cpp Query.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
