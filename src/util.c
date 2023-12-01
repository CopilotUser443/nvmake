#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>

#include "util.h"
#include "defs.h"

void * alloc_sprintf (
    const char * fmt,
    ...
) {
    int iVar1;
    char buffer[256];
    va_list va_args;

va_start (va_args, fmt);
    size_t local_48 = 0;
    void * local_50;

    iVar1 = vsnprintf(buffer, 1, fmt, va_args);


    local_48 = local_48 + (long)iVar1 + 1;
    local_50 = malloc(local_48);
    if (local_50 == (void *)0x0) {
        printf("nvmake: failed to allocate %zu bytes of memory\n", local_48);
        exit(1);
    }
    vsprintf(local_50, fmt, va_args);
va_end (va_args);

    return local_50;
}

void configError (
    const char * fmt,
    ...
) {
    char buffer[4096];
    va_list va_args;

va_start (va_args, fmt);
    vsprintf(buffer, fmt, va_args);
    printf("nvmake: error in build.cfg: %s at line %lu\n", buffer, (ulong)line);
va_end (va_args);

exit(1);
}

void MyChdir (
    const char * fmt,
    ...
) {
    char buffer[4096];
    int chdirstatus = 0;
    va_list va_args;

va_start (va_args, fmt);
    vsprintf(buffer, fmt, va_args);
    chdirstatus = chdir(buffer);
    
    if (chdirstatus == -1) {
        printf("nvmake: Missing Directory: [%s]\n", buffer);
        exit(2);
    }
    if (launchparam  > 0) {
        printf("nvmake: *** Switched directory to: %s ***\n", buffer);
    }
va_end (va_args);

    return;
}

char * DUP_ARGUMENT(char *arg){
    strdup(arg);
    return arg;
}

void expandVar(
    char * param_1,
    char * param_2,
    char * param_3
) {
    size_t sVar1;
    size_t sVar2;
    char *pcVar3;
    size_t sVar4;

    sVar1 = strlen(param_3);
    sVar2 = strlen(param_2);
    pcVar3 = strstr(param_1, param_2);

    if (pcVar3 != (char *)0x0) {
        sVar4 = strlen(pcVar3);
        memmove(pcVar3 + sVar1, pcVar3 + sVar2, (sVar4 - sVar2) + 1);
        memcpy(pcVar3, param_3, sVar1);
    }
    return;
}

char * ExpandVars(
    const char * defined_var
) {
    char * pcVar1;
    strcpy(ExpandVars_szWork, defined_var);
    expandVar(ExpandVars_szWork, "<source>", Global);
    expandVar(ExpandVars_szWork, "<tools>", nv_tools);
    expandVar(ExpandVars_szWork, "<gcomp>", nv_gcomp_root);
    pcVar1 = "0";
    if (currdat != 0) {
        pcVar1 = "1";
    }
    expandVar(ExpandVars_szWork, "<optState>", pcVar1);
    return ExpandVars_szWork;
}

void NormalizeSlashes(
    char * param_1,
    char param_2
) {
    char * local_20;
    for (local_20 = param_1; *local_20 != '\0'; local_20++) {
        if ((*local_20 == '\\') || (*local_20 == '/')) {
            *local_20 = param_2;
        }
    }
    return;
}

int spawn(
    char * binpath, 
    char ** arguments
) {
    int returncode;
    int *lasterrorcode;
    char * local_18;
    char * error_msg;
    char * pcVar5;
    char ** local_20;
    uint local_28;
    pid_t process_pid; 
    pid_t waitprocess_pid;

    if(binpath == (char *)0x0) {
        returncode = -1;
    } else {
        local_20 = arguments;
        local_18 = binpath;
        process_pid = fork();
        pcVar5 = local_18;
        if (process_pid == -1) {
            lasterrorcode = __error();
            error_msg = strerror(*lasterrorcode);
            printf("Failed to fork `%s`: %s\n", pcVar5, error_msg);
            returncode = -1;
        } else {
            if (process_pid == 0) {
                execve(local_18, local_20, *(char ***)environ);
                pcVar5 = local_18;
                lasterrorcode = __error();
                error_msg = strerror(*lasterrorcode);
                printf("Failed to execve `%s`: %s\n", pcVar5, error_msg);
                exit(1);
            }
            waitprocess_pid = waitpid(process_pid, (int *)&local_28, 0);
            pcVar5 = local_18;
            if ((local_28 & 127) == 0) {
                if (((int)local_28 >> 8 & 0xffU) == 0) {
                    if (waitprocess_pid == -1) {
                        lasterrorcode = __error();
                        error_msg = strerror(*lasterrorcode);
                        printf("waitpid() encountered an error: %s\n", error_msg);
                        returncode = -1;
                    } else {
                        returncode = -1;
                    }
                } else {
                    printf("`%s` exited with status: %lu\n", local_18, (ulong)((int)local_28 >> 8 & 0xff));
                    returncode = -1;
                }
            } else {
                lasterrorcode = __error();
                error_msg = strerror(*lasterrorcode);
                printf("`%s` did not terminate normally: %s\n", pcVar5, error_msg);
                returncode = -1;
            }
        }
    }
    return returncode;
}