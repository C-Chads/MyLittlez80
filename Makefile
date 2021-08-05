CC = gcc
AR = ar
# -flto bloats binary size quite a bit but makes it run faster
CFLAGS = -Wall -std=c89 -pedantic -Os -mtune=native -fomit-frame-pointer
INCLUDE_DIR = /usr/local/include
LIB_DIR = /usr/local/lib

all: zextest

tables.h: maketables.c
	$(CC) -Os -Wall -ansi -pedantic $< -o maketables
	./maketables > $@

z80emu.a: tables.h
	$(CC) $(CFLAGS) -c z80emu.c
	$(AR) crs z80emu.a z80emu.o

runner: z80emu.a
	$(CC) $(CFLAGS) main.c z80emu.a -o $@

zextest: runner
	./runner -t testfiles/zexdoc.com
	./runner -t testfiles/zexall.com

install: z80emu.a
	install --mode=444 z80emu.h $(INCLUDE_DIR)/
	install --mode=444 z80emu.a $(LIB_DIR)/

uninstall:
	rm -f $(INCLUDE_DIR)/z80emu.h
	rm -f $(LIB_DIR)/z80emu.a

clean:
	rm -f *.o *.a zextest maketables runner tables.h
