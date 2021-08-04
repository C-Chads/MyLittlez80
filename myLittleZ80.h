/* myLittleZ80.h
 * Header for zextest example.
 *
 * Original Author: David Webster
 *
 * License: Public Domain.
 */

#ifndef __MLZ80_INCLUDED__
#define __MLZ80_INCLUDED__

#include "z80emu.h"

typedef struct MY_LITTLE_Z80 {
	Z80_STATE	state;
	unsigned char	memory[1 << 16];
	int 		is_done;
} MY_LITTLE_Z80;

extern void     out_impl(MY_LITTLE_Z80*zextest, int port, unsigned char x);
extern unsigned char in_impl(MY_LITTLE_Z80*zextest, int port);

#endif
