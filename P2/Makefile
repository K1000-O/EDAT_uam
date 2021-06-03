#
#	MACROS
#

CC=gcc
CFLAGS = -Wall -c

all: menu clean
#
#	Ejecutable
#

menu: menu.o odbc.o query.o
	$(CC) -o $@ $^ -lodbc

#
#	Objetos
#

menu.o: menu.c query.h
	$(CC) $(CFLAGS) $<

odbc.o: odbc.c odbc.h
	$(CC) $(CFLAGS) $<
	
query.o: query.c query.h odbc.h
	$(CC) $(CFLAGS) $<

#
#	Splint
#
splint:
	splint -nullpass *.c *.h > splint.log
	
#
#	Limpiar
#

clean:
	rm -f *.o

clean_all:
	rm -f *.o menu splint.log