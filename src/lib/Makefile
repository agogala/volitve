#----------------------------------------------------------------------------
#	@(#)Makefile	1.1	10/18/96
#
#	Makefile for BB Utils
#----------------------------------------------------------------------------

LIB	= libbb.a
SHLIB	= libbb.$(SOEXT)

FILES	=  Utils 

DEFS    = $(addsuffix .h,$(FILES))
LSRC	= $(addsuffix .cpp,$(FILES))

LSRC	+= .obj/StrSet.o

BUILD	= $(VLIB) $(VSHLIB) 

#----------------------------------------------------------------------------
#	Include macros and targets
#----------------------------------------------------------------------------

include	$(ACE_ROOT)/include/makeinclude/wrapper_macros.GNU
include	$(ACE_ROOT)/include/makeinclude/macros.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.common.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.nonested.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.lib.GNU
include	$(ACE_ROOT)/include/makeinclude/rules.local.GNU


#----------------------------------------------------------------------------
#	Local targets
#----------------------------------------------------------------------------
.obj/StrSet.o: StrSet.cpp
	$(CXX) -c StrSet.cpp -I../include -o .obj/StrSet.o
 
#----------------------------------------------------------------------------
#	Dependencies
#----------------------------------------------------------------------------
# DO NOT DELETE THIS LINE -- g++dep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

.obj/Utils.o .shobj/Utils.so: Utils.cpp

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
