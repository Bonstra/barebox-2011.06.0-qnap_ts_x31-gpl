
#include "tests.h"
#include <diags.h>
#include <common.h>
#include <asm/io.h>
#include <errno.h>
#include <malloc.h>
#include <mach/comcerto-2000.h>
#include <mach/otp.h>

#define	TEST_ADDR_OFFSET	0x1FDF
#define	NO_BYTES	2

/*
Writes a byte then leaves a byte before that and then writes on the previous byte.
--------------------------------------------------------
|  write  | blank  | write  |            | start |
---------------------------------------------------------
*/
int Comcerto_otp_test (struct diags_test_param *p)
{
	int i, ret, count = 8;
	u8 databyte[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
	u8 *buf;
	u8 read_val;

	printf("Comcerto_otp_test.\n");

	for(i = 0; i < NO_BYTES; i++)
	{
		buf = xmalloc(count);

		ret = otp_read(TEST_ADDR_OFFSET + i * 16, buf, count);
		if (!ret)
			printf("Before write: Byte#%d Read=0x%x.\n", i, *buf);
		else
		{
			printf("ERROR: OTP Read has failed\n");
			return ret;
		}

		read_val = *buf;

		printf("Writing OTP...\n");

		otp_write(TEST_ADDR_OFFSET + i * 16, &databyte[0], count);

		printf("OTP write has done.\n");

		ret = otp_read(TEST_ADDR_OFFSET + i * 16, buf, count);
		if (!ret)
			printf("After write: Byte#%d Read=0x%x.\n", i, *buf);
		else
		{
			printf("ERROR: OTP Read has failed\n");
			return ret;
		}

		if(read_val == *buf)
		{
			printf("ERROR: OTP Test has failed\n");
			free(buf);
			return -1;
		}

		free(buf);
	}

	return 0;
}

