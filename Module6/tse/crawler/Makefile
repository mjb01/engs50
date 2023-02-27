LIBSDIR=../lib/
UTILSDIR=../utils/
CFLAGS=-Wall -pedantic -std=c11 -I${UTILSDIR} -L${LIBSDIR} -g
LIBS=-lutils -lcurl
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

valgrind: crawler 
	$(VALGRIND) ./crawler https://thayer.github.io/engs50/ ../pages 1

run: crawler
	./crawler https://thayer.github.io/engs50/ ../pages 1

clean:
	rm -f crawler
	rm -f ../pages/*