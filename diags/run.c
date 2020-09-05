#include <diags.h>
#ifdef CONFIG_COMCERTO_VERIFICATION
#include "dump.h"
#endif

void start_verification(void)
{
	diags_run(NULL);
}

int diags_run (char *name)
{
	unsigned int i;
	struct diags_tests *test;
	struct diags_test_param *params;

	if (name == NULL) {
		 for (i = 0; i < test_list_size; i++) {

			test = test_list + i;
			params = test_param_list + i;

			if(test->init_fnc)
			{
				test->init_fnc();
			}

#ifdef CONFIG_COMCERTO_VERIFICATION
			set_start_mark();
			set_value(test->test_id);
#endif
			if ((*test->test_fnc) (params) == 0)
				printf("Test: %s is PASS\n", test->test_name);
			else
				printf("Test: %s is FAIL\n", test->test_name);
#ifdef CONFIG_COMCERTO_VERIFICATION
			set_end_mark();
#endif
		 }
	} else {

		for (i = 0; i < test_list_size; i++) {
			if (strcmp (test_list[i].test_name, name) == 0)
				break;
		}

		if (i < test_list_size) {

			test = test_list + i;
			params = test_param_list + i;

			if(test->init_fnc && !test->init_flag)
			{
				test->init_fnc();
				test->init_flag = 1;
			}

#ifdef CONFIG_COMCERTO_VERIFICATION
			set_start_mark();
			set_value(test->test_id);
#endif
			if ((*test->test_fnc) (params) == 0)
				printf("Test: %s is PASS\n", test->test_name);
			else
				printf("Test: %s is FAIL\n", test->test_name);
#ifdef CONFIG_COMCERTO_VERIFICATION
			set_end_mark();
#endif
		} else {
			return -1;
		}

	}


	return 0;
}

int diags_info(char *name)
{
	unsigned int test_no;

	if(!test_list_size) {
		printf("No diagnostics tests selected.\n");
		return -1;
	}


	printf("List of diagnostics tests\n");

	for (test_no = 1; test_no <= test_list_size; test_no++)
               printf(" %2d. %s \n", test_no, test_list[test_no - 1].test_name);

	return 0;
}

postcore_initcall(start_verification);
