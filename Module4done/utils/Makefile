CFLAGS = -Wall -pedantic -std=c11 -I -g
OFILES = queue.o hash.o webpage.o pageio.o

all: $(OFILES) 
	ar cr ../lib/libutils.a $(OFILES)

# webpage.o: webpage.c webpage.h
# 	gcc $(CFLAGS) -c $<

%.o: %.c %.h
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o 

