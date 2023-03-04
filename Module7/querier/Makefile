LIBSDIR=../lib/
UTILSDIR=../utils/
CFLAGS=-Wall -pedantic -std=c11 -I${UTILSDIR} -L${LIBSDIR} -g
LIBS=-lutils -lcurl
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

query: query.c
	gcc $(CFLAGS) query.c $(LIBS) -o $@

valgrind: query 
	$(VALGRIND) ./query 

run: query
	./query 

clean:
	rm -f query 