#include "make.h"

int MakeAllModules () {
    int i;
    int result;

    result = 0;
    if ( Global.modules_count <= 0 )
    {
        //
    }
    else
    {
        for ( i = 0; i < Global.modules_count; i++ )
        {
            if ( Global.verboseLevel > 0 )
            {
                printf("nvmake: Making Module @ %s\n", Global.modules[i]);
            }
            MyChdir(Global.modules[i]);
            result = ExecuteGnuMake();
            if ( (Global.make_type & 1) == 0)
            {
                if ( result )
                    break;
            }
        }
    }
    return result;
}

void MyChdir (
    const char * dir,
    ...
) {
    char dirbuf[4096];
    va_list va;
    
    va_start(va, dir);
    vsprintf(dirbuf, dir, va);
    if ( chdir(dirbuf) == -1 )
    {
        fprintf(stderr, "nvmake: Missing Directory: [%s]\n", dirbuf);
        exit(2);
    }
    if ( Global.verboseLevel > 0 )
        printf("nvmake: *** Switched directory to: %s ***\n", dirbuf);
    return;
}

int ExecuteGnuMake()
{
    int i;
    int result;
    int status;
    time_t start_time;
    time_t ending_time;

    if ( Global.GMAKE_TIMER )
    {
        printf("nvmake: ********** Start Timer **********\n");
        time(&start_time);
    }
    if ( Global.verboseLevel > 0 )
    {
        printf("nvmake: GnuMake Path: [%s]\n", Global.GMAKE);
        printf("nvmake: GnuMake Args:\n");
        for ( i = 1; i < Global.argCount; i++ )
            printf("         [%s]\n", Global.args[i]);
        printf("\n");
    }
    Global.args[Global.argCount] = (char*)0;
    result = spawn(Global.GMAKE, Global.args);
    if ( result == -1 )
    {
        fprintf(stderr, "=======================================\n");
        fprintf(stderr, "Failed to execute gnumake\n");
        fprintf(stderr, "  Command: %s\n", Global.GMAKE);
        fprintf(stderr, "     Args:\n");
        for ( i = 1; i < Global.argCount; ++i )
            fprintf(stderr, "         [%s]\n", Global.args[i]);
        status = errno;
        fprintf(stderr, "   Status: %d\n", status);
        fprintf(stderr, "=======================================\n");
    }
    if ( Global.GMAKE_TIMER )
    {
        time(&ending_time);
        printf(
            "nvmake: ********** Elapsed Time %02ld:%02ld (%ld seconds)**********\n",
            (ending_time - start_time) / 60,
            (ending_time - start_time) % 60,
            (ending_time - start_time));
    }
    return result;
}