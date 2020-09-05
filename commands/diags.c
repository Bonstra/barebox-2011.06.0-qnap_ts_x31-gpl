#include <common.h>
#include <command.h>
#include <getopt.h>
//#include <diags.h>

extern int diags_info(char *name);
extern int diags_run(char *name);

static int do_diags(struct command *cmdtp, int argc, char *argv[])
{

	int opt;

	if((opt = getopt(argc, argv, "lai:r:")) > 0)
	{
		switch(opt) {
			case 'l':
				diags_info (NULL);
				break;
			case 'i':
				if (diags_info (optarg) != 0)
					printf ("%s - no such test\n", optarg);
				break;
			case 'r':
				if (diags_run (optarg) != 0)
					printf ("%s - unable to execute the test\n", optarg);
				break;
			case 'a':
				diags_run (NULL);
				break;
			default:
				printf ("%s - options not supported\n");
				return -1;
		}
	}

	return 0;
}

static const __maybe_unused char cmd_diags_help[] =
"Usage: diag [OPTIONS] [<test>]\n"
"\n"
"options:\n"
" -l \n"
"    - print list of available hardware tests\n"
" -i <test>\n"
"         - print information about specified hardware test\n"
" -r <test>\n"
"         - run specified hardware test\n"
" -a\n"
"         - run all hardware test\n";


BAREBOX_CMD_START(diags)
        .cmd            = do_diags,
        .usage          = "diag - perform hardware tests",
        BAREBOX_CMD_HELP(cmd_diags_help)
BAREBOX_CMD_END

