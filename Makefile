CC = gcc
AR = ar
CFLAGS = -flto -Wall -std=c89 -pedantic -Os -mtune=native -fomit-frame-pointer
INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib

all: zextest

tables.h: maketables.c
	$(CC) -Os -Wall -ansi -pedantic $< -o maketables
	./maketables > $@

zextest: tables.h
	$(CC) $(CFLAGS) -DDO_ZEXTEST z80emu.c zextest.c -o $@

z80emu.a: tables.h
	$(CC) $(CFLAGS) -c z80emu.c
	$(AR) crs z80emu.a z80emu.o

runner: z80emu.a
	$(CC) $(CFLAGS) main.c z80emu.a -o $@

install: z80emu.a
	install --mode=444 z80emu.h $(INCLUDE_DIR)/
	install --mode=444 z80emu.a $(LIB_DIR)/

uninstall:
	rm -f $(INCLUDE_DIR)/z80emu.h
	rm -f $(LIB_DIR)/z80emu.a

clean:
	rm -f *.o *.a zextest maketables runner
