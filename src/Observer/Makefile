#----------------------------------------------------------------------------
# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: Makefile 1.7 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
#
# Makefile za opazovalca.
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
#	Local macros
#----------------------------------------------------------------------------

BIN   	= observerd

FILES   = Notification_Handler \
	  Formater \
	  State

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
LDFLAGS += -R$(ACE_ROOT)/ace -R../lib
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

.obj/.obj/config.o .shobj/config.so: $(PYTHON_ROOT)/lib/python1.4/config/config.c
	$(CC) $(CFLAGS) $(INCLDIRS) -DHAVE_CONFIG -DNO_MAIN -c $(PYTHON_ROOT)/lib/python1.4/config/config.c -o .obj/config.o

 
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------

# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Notification_Handler.o .shobj/Notification_Handler.so: Notification_Handler.cpp \
 Notification_Handler.h State.h ../include/StrSet.h ../include/Utils.h \
 Formater.h ../include/Config.h
.obj/Formater.o .shobj/Formater.so: Formater.cpp ../include/Config.h Formater.h State.h \
 ../include/StrSet.h ../include/Utils.h
.obj/State.o .shobj/State.so: State.cpp State.h

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
