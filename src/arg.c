#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

#include "defs.h"
#include "util.h"
#include "arg.h"

void AddArgument (
    const char * fmt,
    ...
) {
    int iVar1;
    char buffer[4096];
    char *local_1040 = NULL;
    char *local_1038;
    long local_10;
    va_list va_args;

va_start(va_args, fmt);
    vsprintf(buffer, fmt, va_args);
    for (local_1038 = local_1040; *local_1038 != '\0'; local_1038++) {
        iVar1 = isspace((int)*local_1038);
        if (iVar1 != 0) {
            *local_1038 = '\0';
            if (local_1040 != local_1038) {
                AddLiteralArgument(local_1040);
            }
            local_1040 = local_1038 + 1;
        }
    }
    AddLiteralArgument(local_1040);
va_end(va_args);

    return;
}

void AddEarlyArgument (
    const char * fmt,
    ...
) {
    char buffer[4096];
    int itteration;
    va_list va_args;

va_start(va_args, fmt);
    vsprintf(buffer, fmt, va_args);
    if (argi > 255) {
        printf("nvmake: exceeded maximum argument count!\n");
        exit(98);
    }
    argi++;
    for (itteration = argi; itteration > 1; itteration--) {
        args[itteration] = args[itteration - 1];
    }
va_end(va_args);
    return;
}

void AddLiteralArgument (char * argument) {
    char * uVar1;
    long lVar2;

    if (argi > 255) {
        printf("nvmake: exceeded maximum argument count!\n");
        //Same error, different exit codes????
        exit(99);
    }
    uVar1 = DUP_ARGUMENT(argument);
    lVar2 = (long)argi;
    argi++;
    args[lVar2] = uVar1;
    return;
}

void RemoveOneArgument(void) {

    if (argi > 0) {
        argi--;
    }
    return;
}