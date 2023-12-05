#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

#ifdef __unix__

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
extern char **environ;
#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)

#endif

#ifdef _WIN32
#include "unistd.w.h"
extern char **_environ;
#endif

extern int line;
extern int debug;


char * alloc_sprintf(char * fmt, ...)
{
    int result;
    char buffer_alloc;
    char * buffer;
    size_t buffer_size;
    char * tmp_buffer;
    va_list va;

    tmp_buffer = fmt;
    buffer_size = 0LL;

    va_start(va, fmt);
    result = vsnprintf(&buffer_alloc, 1uLL, fmt, va);
    buffer_size += result;
    buffer = (char *)malloc(buffer_size++);
    
    if (!buffer)
    {
        fprintf(stderr, "nvmake: failed to allocate %ld bytes of memory\n", buffer_size);
        exit(1);
    }
    va_end(va);

    va_start(va, fmt);
    vsprintf(buffer, tmp_buffer, va);
    return buffer;
}

void NormalizeSlashes(char * path, char replacement)
{
    while ( true )
    {
        if ( *path == '\0') {
            break;
        }
        if (*path == '/' || *path == '\\')
            *path = replacement;
        path++;
    }
}

void configError(char * fmt, ...)
{
    char buffer[4096];
    va_list va;

    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    fprintf(stderr, "nvmake: error in build.cfg: %s at line %d\n", buffer, line);
    exit(1);
    return;
}


void MyChdir(const char * path, ...)
{
    char buffer[4096];
    va_list va;

    va_start(va, path);
    vsprintf(buffer, path, va);
    if (chdir(buffer) == -1)
    {
        fprintf(stderr, "nvmake: Missing Directory: [%s]\n", buffer);
        exit(2);
    }
    if (debug > 0)
    {
        printf("nvmake: *** Switched directory to: %s ***\n", buffer);
    }
    return;
}

unsigned int spawn(
    char * binpath,
    const char ** arguments
) {
#ifdef __unix__
    pid_t forked_process;
    pid_t wait_process;
    int wait_token;
    unsigned int status = 0;

    if (binpath)
    {
        forked_process = fork();
        if (forked_process == -1)
        {
            status = -1;
            fprintf(stderr, "Failed to fork `%s`: %s\n", binpath, strerror(errno));
        }
        else 
        {
            if (!forked_process)
            {
                execve(binpath, arguments, environ);
                fprintf(stderr, "Failed to execve `%s`: %s\n", binpath, strerror(errno));
                exit(1);
            }
            wait_process = waitpid(forked_process, &wait_token, 0);
            if ((wait_token & 0x7F) != 0)
            {
                fprintf(stderr, "`%s` did not terminate normally: %s\n", binpath, strerror(errno));
            }
            else if (BYTE1(wait_token))
            {
                fprintf(stderr, "`%s` exited with status %d\n", binpath, BYTE1(wait_token));
                return -1U;
            }
            else if (wait_process == -1)
            {
                fprintf(stderr, "waitpid() encountered an error: %s\n", strerror(errno));
                return -1U;
            }
            else
            {
                return 0;
            }
        }    
    }
    else
    {
        return -1U;
    }
    return status;
#endif
#ifdef _WIN32
    //We can't fork a process under windows
    int64_t status;
    status = _spawnve(P_WAIT, binpath, arguments, _environ);
    if (errno) {
        fprintf(stderr, "`%s` did not terminate normally: %s\n", binpath, strerror(errno));
        return -1U;
    }
    if (status != -1 && status != 0) {
        fprintf(stderr, "`%s` exited with status %d\n", binpath, status);
        return -1U;
    }
    if (status == -1 ) {
        fprintf(stderr, "spawnve() encountered an error: %s\n", strerror(errno));
        return -1U;
    }
    return 0;
#endif
}

//TODO: Reverse engineer this function
/*
bool getToken(
    char * tok,
    char ** array
) {
    char *v2;
    char *v3;
    char *v4;
    char *v5;
    char *v6;
    char *v7;
    bool v9;
    bool v10;
    bool v11;
    bool v12;
    bool v13;
    char * tmptok;
    while ( 1 )
    {
        v13 = 0;
        if (**array)
        {
            v12 = 1;
            if (!isspace((char)**array))
            {
                v12 = **array == '\n';
            }
            v13 = v12;
        }
        if (!v13) break;
    }
    if (**array == '\"')
    {
        while (1)
        {
            v11 = 0;
            if (**array)
            {
                v11 = **array != '\"';
            }
            if (!v11) break;
                v2 = (*array)++;
                v3 = tmptok++;
                *v3 = *v2;
        }
        if (**array != '\"')
        {
            fprintf(stderr, "nvmake: expected ending quote in config file\n");
            exit(1);
        }
    }
    else if (iscsym((char)**array))
    {
        while (1)
        {
            v10 = 0;
            if (**array)
                v10 = iscsym((char)**array);
            if (!v10) break;
            v4 = (*array)++;
            v5 = tmptok++;
            *v5 = *v4;
        }
    }
    else
    {
        while (1)
        {
            v9 = 0;
            if (**array)
            {
                v9 = 0;
                if (!iscsym((char)**array))
                {
                    v9 = **array != '\"';
                }
            }
            if (!v9) break;
            v6 = (*array)++;
            v7 = tmptok++;
            *v7 = *v6;
        }
    }
    *tmptok = 0;
    return tok != tmptok;
}*/

int getToken(
    char * param_1,
    char ** param_2
) {
    char * pcVar1;
    int iVar2;
    bool bVar3;
    char * local_10 = malloc(1024);

    while ( true ) {
        bVar3 = false;
        if (**param_2 != '\0') {
            iVar2 = isspace((int)**param_2);
            bVar3 = true;
            if (iVar2 == 0) {
                bVar3 = **param_2 == '\n';
            }
        }
        if (!bVar3) break;
        pcVar1 = *param_2;
        *param_2 = pcVar1 + 1;
        *local_10 = *pcVar1;
        local_10++;
    }

    if (**param_2 == '\"') {
        pcVar1 = *param_2;
        *param_2 = pcVar1 + 1;
        *local_10 = *pcVar1;
        local_10++;
        local_10 = param_1;
        while ( true ) {
            bVar3 = false;
            if (**param_2 != '\0') {
                bVar3 = **param_2 != '\"';
            }
            if (!bVar3) break;
            pcVar1 = *param_2;
            *param_2 = pcVar1 + 1;
            *local_10 = *pcVar1;
            local_10++;
        }
        if (**param_2 != '\"') {
            printf("nvmake: expected ending quote in config file\n");
            exit(1);
        }
        pcVar1 = *param_2;
        *param_2 = pcVar1 + 1;
        *local_10 = *pcVar1;
        local_10++;
    }
    else {
        iVar2 = iscsym((int)**param_2);
        local_10 = param_1;
        if (iVar2 == 0) {
            while ( true ) {
                bVar3 = false;
                if (**param_2 != '\0') {
                    iVar2 = iscsym((int)**param_2);
                    bVar3 = false;
                    if (iVar2 == 0) {
                        bVar3 = **param_2 != '\"';
                    }
                }
                if (!bVar3) break;
                pcVar1 = *param_2;
                *param_2 = pcVar1 + 1;
                *local_10 = *pcVar1;
                local_10++;
            }
        }
        else {
            while ( true ) {
                bVar3 = false;
                if (**param_2 != '\0') {
                    iVar2 = iscsym((int)**param_2);
                    bVar3 = iVar2 != 0;
                }
                if (!bVar3) break;
                pcVar1 = *param_2;
                *param_2 = pcVar1 + 1;
                *local_10 = *pcVar1;
                local_10++;
            }
        }
    }
    *local_10 = '\0';
    return param_1 != local_10;
}





















