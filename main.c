/* MY_LITTLE_Z80.c
 * Example program using z80emu to run the zexall and zexdoc tests. This will 
 * check if the Z80 is correctly emulated.
 *
 * Original Authors: Lin Ke-Fong, Chris Pressey
 *
 * License: Public Domain.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myLittleZ80.h"
#include "z80emu.h"



#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED)
#define MAXIMUM_STRING_LENGTH   100

static void	emulate (char *filename);
int main (int argc, char** argv)
{
	emulate(argv[1]);
    return EXIT_SUCCESS;
}

/* Emulate "zexdoc.com" or "zexall.com". */

static void emulate (char *filename)
{
        FILE   	*file;
        long   	l;
	MY_LITTLE_Z80	context;
        double 	total;

        printf("Testing \"%s\"...\n", filename);
        if ((file = fopen(filename, "rb")) == NULL) {

                fprintf(stderr, "Can't open file!\n");
                exit(EXIT_FAILURE);

        }
        fseek(file, 0, SEEK_END);
        l = ftell(file);

        fseek(file, 0, SEEK_SET);
        fread(context.memory + 0x100, 1, l, file);
        fclose(file);
        /* Emulate. */
		Z80Reset(&context.state);
		context.state.pc = 0x100;
		total = 0.0;
	do{
		total += Z80Emulate(&context.state, CYCLES_PER_STEP, &context);
	}while(context.state.status != Z80_STATUS_HALT);
}

/* Emulate CP/M bdos call 5 functions 2 (output character on screen) and 9
 * (output $-terminated string to screen).
 */

void out_impl(MY_LITTLE_Z80* mlz80, int port, unsigned char x)
{
        if (zextest->state.registers.byte[Z80_C] == 2)
                putchar(zextest->state.registers.byte[Z80_E]);
        else if (zextest->state.registers.byte[Z80_C] == 9) {
                int     i;
                for (i = zextest->state.registers.word[Z80_DE];zextest->memory[i] != '$';i++) {
                        putchar(zextest->memory[i & 0xffff]);
                }
        }
}


unsigned char in_impl(MY_LITTLE_Z80*MY_LITTLE_Z80, int port)
{
	(void)MY_LITTLE_Z80;
	(void)port;
   return 0; /*TODO*/
}
