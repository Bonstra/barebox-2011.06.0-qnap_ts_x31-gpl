#include <common.h>
#include <command.h>
#include <errno.h>

static int do_copy(struct command *cmdtp, int argc, char *argv[])
{
	volatile u32 *src, *dst;
	int count;
	int i;

        if(argc < 3)
                return COMMAND_ERROR_USAGE;


	src =  simple_strtoul(argv[1], NULL, 0);
	dst =  simple_strtoul(argv[2], NULL, 0);
	count =  simple_strtoul(argv[3], NULL, 0);
	
	for(i = 0 ; i < count; )
	{
		*dst = *src;
		 dst++;
		 src++;
		 i+=4;
	}
	
}

static const __maybe_unused char cmd_copy_help[] =
"Usage:\n"
"\n"
"copy <src_addr> <dst_addr> <count>\n"
;


BAREBOX_CMD_START(copy)
        .cmd            = do_copy,
        .usage          = "copy command",
        BAREBOX_CMD_HELP(cmd_copy_help)
BAREBOX_CMD_END

