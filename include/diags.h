#ifndef _COMCERTO_DIAG_H
#define _COMCERTO_DIAG_H

#ifndef NULL
#define NULL    0
#endif

#define MAX_DIAG_PARAMS             8

struct diags_test_param {
        unsigned int test_id;
	int no_params;
        int params[MAX_DIAG_PARAMS];
};

struct diags_tests {
        unsigned int test_id;
        char *test_name;
        int (*test_fnc) (struct diags_test_param *p);
        int (*init_fnc) (void);
	int init_flag;
        char *desc;
	char *usage;
};

int diags_run (char *name);
int diags_info (char *name);
extern struct diags_tests test_list[];
extern struct diags_test_param test_param_list[];
extern unsigned int test_list_size;
#endif
