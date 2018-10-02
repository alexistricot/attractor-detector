CC = gcc
CFLAGS = -L smchr
DEPS = premierjet.h
LIBS = -lsmchr -lm -ldl

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

premierjet: automata.o transition.o simplify.o getData.o premierjet.o init.o autolists.o frequency.o printAN.o writeSolver.o attractor.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o
