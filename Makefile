#CC = gcc
CFLAGS = -Wall -ansi -pedantic -O2 -march=native -fomit-frame-pointer

all: zextest

tables.h: maketables.c
	$(CC) -Os -Wall -ansi -pedantic $< -o maketables
	./maketables > $@

zextest: tables.h
	$(CC) $(CFLAGS) -DDO_ZEXTEST z80emu.c zextest.c -o $@

mylittlez80:
	$(CC) $(CFLAGS) z80emu.c main.c -o $@

clean:
	rm -f *.o zextest maketables mylittlez80
