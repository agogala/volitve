# Makefile for list-fifo.cc

LIBPQDIR=/home/postgres/pgsql/lib
HEADERDIR=/home/postgres/pgsql/include

CXXFLAGS= $(CFLAGS) -Wno-error

INCLUDE_OPT= -I$(HEADERDIR)

CXXFLAGS+= $(INCLUDE_OPT)

LDLIBS+= -L$(LIBPQDIR) -lpq++ -lpq 

list-fifo: list-fifo.cc

#list-fifo.o: list-fifo.cc