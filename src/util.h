void * alloc_sprintf (
    const char * fmt,
    ...
);

void configError (
    const char * fmt,
    ...
);

void MyChdir (
    const char * fmt,
    ...
);

void expandVar(
    char * param_1,
    char * param_2,
    char * param_3
);

char * ExpandVars(
    const char * defined_var
);

void NormalizeSlashes(
    char * param_1,
    char param_2
);

char * DUP_ARGUMENT(char *arg);

int spawn(
    char * binpath, 
    char **arguments
);