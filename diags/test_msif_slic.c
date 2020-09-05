#include <common.h>
#include <asm/io.h>
#include "tests.h"
#include "diags.h"
#include <mach/gpio.h>
#include <mach/comcerto-2000.h>

extern int proslic_init(void);

void Comcerto_msif_slic_test(struct diags_test_param *p)
{
	printf("MSIF/SLIC Diag Test...\n");

	proslic_init();
}

