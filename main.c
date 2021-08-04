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
#include "z80emu.h"

#define Z80_CPU_SPEED           4000000   /* In Hz. */
#define CYCLES_PER_STEP         (Z80_CPU_SPEED)
#define MAXIMUM_STRING_LENGTH   100

static int is_testing = 0;
static void	emulate (char *filename);
static void	emulate_test(char *filename);
int main (int argc, char** argv)
{int i;
	for(i = 1; i < argc; i++){
		if(argv[i-1][0] == '-' && argv[i-1][1] == 't')
			{is_testing = 1;emulate_test(argv[i]);return EXIT_SUCCESS;}
		else if(argv[i-1][0] == '-' && argv[i-1][1] == 'r')
			{emulate(argv[i]);return EXIT_SUCCESS;}
	}
    return EXIT_FAILURE;
}

/* Emulate "zexdoc.com" or "zexall.com". */

static void emulate_test(char *filename)
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

   		context.memory[0] = 0xd3;       /* OUT N, A */
		context.memory[1] = 0x00;
		context.memory[5] = 0xdb;       /* IN A, N */
		context.memory[6] = 0x00;
		context.memory[7] = 0xc9;       /* RET */
        /* Emulate. */
		Z80Reset(&context.state);
		context.state.pc = 0x100;
		total = 0.0;
	do{
		total += Z80Emulate(&context.state, CYCLES_PER_STEP, &context);
	}while(context.state.status != Z80_STATUS_HALT);
}


static void emulate(char *filename)
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

/*
	TEMPORARY
*/
void out_impl(MY_LITTLE_Z80* mlz80, int port, unsigned char x)
{
	/*TODO*/
	if(is_testing)
		mlz80->state.status = Z80_STATUS_HALT;
	(void)port;
}

/* Emulate CP/M bdos call 5 functions 2 (output character on screen) and 9
 * (output $-terminated string to screen).
 */
unsigned char in_impl(MY_LITTLE_Z80* mlz80, int port)
{
	if (mlz80->state.registers.byte[Z80_C] == 2)
		putchar(mlz80->state.registers.byte[Z80_E]);
	else if (mlz80->state.registers.byte[Z80_C] == 9) {
		int     i;
		for (i = mlz80->state.registers.word[Z80_DE];mlz80->memory[i] != '$';i++) {
			putchar(mlz80->memory[i & 0xffff]);
		}
	}
	return 0;
}
