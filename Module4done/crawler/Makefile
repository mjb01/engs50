LIBSDIR=../lib/
UTILSDIR=../utils/
CFLAGS=-Wall -pedantic -std=c11 -I${UTILSDIR} -L${LIBSDIR} -g
LIBS=-lutils -lcurl
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: crawler indexer

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

indexer: indexer.c $(UTILSDIR)/pageio.c $(UTILSDIR)/webpage.c
	gcc $(CFLAGS) indexer.c $(UTILSDIR)/pageio.c $(UTILSDIR)/webpage.c -o $@

valgrind: crawler 
	$(VALGRIND) ./crawler https://thayer.github.io/engs50/ pages 0

run: crawler
	./crawler https://thayer.github.io/engs50/ pages 1

clean:
	rm -f crawler
	rm -f indexer
	rm -f ../pages/*

