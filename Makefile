#
#	MACROS
#

CC=gcc
CFLAGS = -Wall -c

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
#	Limpiar
#

clean:
	rm *.o