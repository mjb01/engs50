LIBSDIR=../lib/
UTILSDIR=../utils/
CFLAGS=-Wall -pedantic -std=c11 -I${UTILSDIR} -L${LIBSDIR} -g
LIBS=-lutils -lcurl
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: indexer.c
	gcc $(CFLAGS) indexer.c $(LIBS) -o $@

valgrind: indexer 
	$(VALGRIND) ./indexer ../pages indexnm

run: indexer
	./indexer ../pages indexnm

clean:
	rm -f indexer indexnm