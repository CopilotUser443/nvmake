#include "arg.h"

char * AddArgument (
    const char * args,
    ...
) {
    char * arg;
    char * Itteration;
    char argBuf[4096];
    va_list va;

    va_start(va, args);
    vsprintf(argBuf, args, va);
    arg = argBuf;
    for ( Itteration = argBuf; *Itteration; Itteration++)
    {
        if ( isspace(*Itteration) )
        {
            *Itteration = 0;
            if ( arg != Itteration )
                AddLiteralArgument(arg);
            arg = Itteration + 1;   
        }
    }
    return AddLiteralArgument(arg);
}

char * AddLiteralArgument (
    char * arg
) {
    char * result;
    int argi;

    if ( Global.argCount >= 256 )
    {
        fprintf(stderr, "nvmake: exceeded maximum argument count!\n");
        exit(99);
    }
    result = DUP_ARGUMENT(arg);
    argi = Global.argCount;
    Global.argCount = argi + 1;
    Global.args[argi] = result;
    return result;
}

void AddEarlyArgument (
    const char * arg,
    ...
) {
    int i;
    char argbuf[4096];
    va_list va;

    va_start(va, arg);
    vsprintf(argbuf, arg, va);
    if ( Global.argCount >= 256 )
    {
        fprintf(stderr, "nvmake: exceeded maximum argument count!\n");
        exit(98);
    }
    for ( i = Global.argCount++; i >= 0; i-- )
        Global.args[i + 1] = Global.args[i];
    Global.args[0] = strdup(argbuf);
    return;
}

void RemoveOneArgument()
{
    if ( Global.argCount > 0 )
    {
        Global.args[Global.argCount - 1] = "\0";
        Global.argCount--;
    }
    return;
}

char * DUP_ARGUMENT (
    const char * arg
) {
    return strdup(arg);
}