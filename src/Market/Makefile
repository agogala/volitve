#----------------------------------------------------------------------------
# $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
#
# $Id: Makefile 1.11 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
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

LSRC	+= .obj/Stanje.o .obj/VectorRequest.o

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD	= $(VBIN)

INCLDIRS += -I$(PG_ROOT)/include

LDLIBS += -lpq++ -lpq -lg++ -lstdc++ -lbb

LDFLAGS += -L$(PG_ROOT)/lib -L../lib 

#LIBS 	+= /usr/local/lib/ccmalloc.o
LIBS	+= .obj/Stanje.o

# Tole potrebujemo samo zaradi debugiranja na Solarisu.
ifeq ($(OS),SunOS)
LDFLAGS += -R$(ACE_ROOT)/ace
endif

ifeq ($(OS),Linux)
LDFLAGS += -Xlinker -rpath -Xlinker ../lib
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

.obj/Stanje.o .shobj/Stanje.so: Stanje.cpp
	$(CXX) -g $(INCLDIRS) -c Stanje.cpp -o .obj/Stanje.o

#VectorRequest.o: VectorRequest.cpp
#	$(CXX) -g $(INCLDIRS) -c VectorRequest.cpp -o .obj/VectorRequest.o

#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Market_Handler.o .shobj/Market_Handler.so: Market_Handler.cpp marketd.h Market.h Request.h \
 ../include/Config.h ../include/StrSet.h ../include/Utils.h \
 Market_Handler.h Notifier.h
.obj/Market.o .shobj/Market.so: Market.cpp VectorRequest.h Request.h ../include/Config.h \
 Market.h ../include/StrSet.h ../include/Utils.h Query.h
.obj/Request.o .shobj/Request.so: Request.cpp Request.h ../include/Config.h Query.h \
 MarketErrors.h ../include/Utils.h
.obj/VectorRequest.o .shobj/VectorRequest.so: VectorRequest.cpp VectorRequest.h Request.h \
 ../include/Config.h
.obj/Notifier.o .shobj/Notifier.so: Notifier.cpp marketd.h Notifier.h ../include/Config.h
.obj/Query.o .shobj/Query.so: Query.cpp Query.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
