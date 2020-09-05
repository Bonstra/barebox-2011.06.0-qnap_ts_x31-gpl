#include "dump.h"

#define DUMP_START_ADDR	0x8300FF00
#define START_MARKER	0xE0E0E0E0
#define END_MARKER	0xEFEFEFEF
#define PASS_VALUE	0x1111111
#define FAIL_VALUE	0xDEADDEAD

int *current_addr = (int *)DUMP_START_ADDR;

void set_start_mark()
{
	*(volatile int *)current_addr = START_MARKER;
	current_addr++;
}

void set_end_mark()
{
        *(volatile int *)current_addr = END_MARKER;
        current_addr++;
}

void set_value(int val)
{
	*(volatile int *)current_addr = val;
	current_addr++;
}

void set_pass()
{
	*(volatile int *)current_addr = PASS_VALUE;
	current_addr++;
}

void set_fail()
{
	*(volatile int *)current_addr = FAIL_VALUE;
	current_addr++;
}

int *get_current_loc()
{
	return current_addr;
}

void set_addr_val(int *addr, int val)
{
	*(volatile int *)addr = val;
}
