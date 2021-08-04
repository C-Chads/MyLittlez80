/* zextest.h
 * Header for zextest example.
 *
 * Original Author: Lin Ke-Fong
 *
 * License: Public Domain.
 */

#ifndef __ZEXTEST_INCLUDED__
#define __ZEXTEST_INCLUDED__

#include "z80emu.h"

typedef struct ZEXTEST {
	Z80_STATE	state;
	unsigned char	memory[1 << 16];
	int 		is_done;
} ZEXTEST;

extern void     SystemCall (ZEXTEST *zextest);

#endif
