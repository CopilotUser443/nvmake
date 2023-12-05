#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "arg.h"

extern int argi;
extern char ** args;

void AddArgument(const char *fmt, ...)
{
    char * arg;
    char * i;
    char buffer[4096];
    va_list va;

    va_start(va, fmt);
    vsprintf(buffer, fmt, va);
    arg = buffer;
    for (i = buffer; *i; i++)
    {
        if (isspace(*i)) {
            *i = 0;
            if (arg != i) {
                //stub: AddLiteralArgument(arg);
            }
            arg = i + 1;
        }
    }
    //stub: AddLiteralArgument(arg);
    return;
}

void AddEarlyArgument(const char *arg, ...)
{
    int itteration;
    char buffer[4096];
    va_list va;

    va_start(va, arg);
    vsprintf(buffer, arg, va);

    if (argi >= 256)
    {
        fprintf(stderr, "nvmake: exceeded maximum argument count!\n");
        exit(98);
    }

    for (itteration = argi + 1; itteration > 1; itteration--)
    {
        args[itteration] = args[itteration - 1];
    }
    args[1] = strdup(buffer);
    return;
}

char * DUP_ARGUMENT(const char *arg)
{
    return strdup(arg);
}

void AddLiteralArgument(char * argument)
{
    char * result;
    int arg_count;
    if (argi >= 256) {
        fprintf(stderr, "nvmake: exceeded maximum argument count!\n");
        exit(99);
    }
    result = DUP_ARGUMENT(argument);
    arg_count = argi;
    argi = arg_count + 1;
    args[arg_count + 1] = result;
    return;
}

void RemoveOneArgument()
{
    if (argi > 0)
    {
        argi--;
    }
    return;
}