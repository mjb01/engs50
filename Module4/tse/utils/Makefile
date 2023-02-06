OFILES=queue.o hash.o webpage.o
CFLAGS=-Wall -pedantic -std=c11 -I. 

all: $(OFILES)
	ar cr /thayerfs/home/f004r8p/engs50/Modules/Module4/tse/utils/lib/libutils.a $(OFILES)

webpage.o: webpage.c webpage.h
	gcc $(CFLAGS) -c webpage.c

clean:
	rm -f *.g /thayerfs/home/f004r8p/engs50/Modules/Module4/tse/utils/lib/libutils.a
