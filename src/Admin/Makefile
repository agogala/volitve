#----------------------------------------------------------------------------
# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: Makefile 1.4 Fri, 03 Oct 1997 15:45:58 +0000 andrej $
#
# Makefile za opazovalca.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= admind

FILES   = Peer_Handler \
	  Registrator

LSRC	= $(addsuffix .cpp,$(FILES))
LOBJ	= $(addsuffix .o,$(FILES))
SHOBJ	= $(addsuffix .so,$(FILES))

LSRC    += .obj/config.o

LDLIBS	= $(addprefix $(VSHDIR),$(SHOBJ))

VLDLIBS = $(LDLIBS:%=%$(VAR))

BUILD 	= $(VBIN)

INCLDIRS += -I../include

LDFLAGS  += .obj/config.o -L$(PYTHON_ROOT)/lib/python1.4/config -lModules -lPython -lObjects -lParser -lm $(PYTHON_ROOT)/lib/python1.4/config/getpath.o
INCLDIRS += -I$(PYTHON_ROOT)/include/python1.4 -I$(PYTHON_ROOT)/lib/python1.4/config

LDLIBS  += -lpq++ -lpq -lg++ -lbb
LDFLAGS += -L$(PG_ROOT)/lib -L../lib

ifeq ($(OS),Linux)
LDLIBS  += -lreadline -ltermcap
LDFLAGS += -rdynamic
endif

# Tole potrebujemo samo zaradi debugiranja na Solarisu.
ifeq ($(OS),SunOS)
LDFLAGS += -R$(ACE_ROOT)/ace
endif

#LDFLAGS += $(platf_ldflags)

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

.obj/config.o .shobj/config.so: $(PYTHON_ROOT)/lib/python1.4/config/config.c
	$(CC) $(CFLAGS) $(INCLDIRS) -DHAVE_CONFIG -DNO_MAIN -c $(PYTHON_ROOT)/lib/python1.4/config/config.c -o .obj/config.o

 
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Peer_Handler.o .shobj/Peer_Handler.so: Peer_Handler.cpp admind.h Peer_Handler.h \
 ../include/Config.h Registrator.h ../include/Utils.h
.obj/Registrator.o .shobj/Registrator.so: Registrator.cpp ../include/Config.h Registrator.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
