LIBSDIR=../lib/
UTILSDIR=../utils/
CFLAGS=-Wall -pedantic -std=c11 -I${UTILSDIR} -L${LIBSDIR} -g
LIBS=-lutils -lcurl
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test.c
	gcc $(CFLAGS) test.c $(LIBS) -o $@

valgrind: test 
	$(VALGRIND) ./test 

run: test
	./test 

clean:
	rm -f test *.o 