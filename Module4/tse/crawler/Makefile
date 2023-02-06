CFLAGS= -Wall -pedantic -std=c11 -I/thayerfs/home/f004r8p/engs50/Modules/Module4/tse/utils/ -L/thayerfs/home/f004r8p/engs50/Modules/Module4/tse/utils/lib -g
LIBS=-lutils -lcurl

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

clean:
	rm -f crawler
