#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "util.h"

extern int argi;
extern int launchm;
extern int debug;
extern int make_conditions;
extern int module_conditions;
extern char * makepath;
extern const char ** args;
extern const char ** modules;

unsigned int ExecuteGnuMake()
{
    int itteration;
    unsigned int spawn_return;
    time_t start_time;
    time_t finish_time;

    if (launchm) 
    {
        printf("nvmake: ********** Start Timer **********\n");
        time(&start_time);
    }
    if (debug > 0)
    {
        printf("nvmake: GnuMake Path: [%s]\n", makepath);
        printf("nvmake: GnuMake Args:\n");
        for (itteration = 1; itteration < argi; itteration++) 
        {
            printf("         [%s]\n", args[itteration]);
        }
        printf("\n");
    }
    args[argi] = 0LL;
    spawn_return = spawn(makepath, args);
    if (spawn_return == -1U)
    {
        fprintf(stderr, "=======================================\n");
        fprintf(stderr, "Failed to execute gnumake\n");
        fprintf(stderr, "  Command: %s\n", makepath);
        fprintf(stderr, "     Args:\n");
        for (itteration = 1; itteration < argi; itteration++)
        {
            fprintf(stderr, "         [%s]\n", args[itteration]);
        }
        fprintf(stderr, "   Status: %d\n", errno);
        fprintf(stderr, "=======================================\n");
    }
    if (launchm)
    {
        time(&finish_time);
        printf("nvmake: ********** Elapsed Time %02ld:%02ld (%d seconds)**********\n",
                (finish_time - start_time) / 60,
                (finish_time - start_time) & 60,
                (unsigned int)(finish_time - start_time));
    }
    return spawn_return;
}

unsigned int MakeAllModules(void) 
{    
    int itteration;
    unsigned int result = 0;

    if (make_conditions <= 0)
    {
        return (unsigned int)ExecuteGnuMake();
    }
    else
    {
        for (itteration = 0; itteration < make_conditions; itteration++)
        {
            if (debug > 0)
            {
                printf("nvmake: Making Module @ %s\n", modules[itteration]);
            }
            MyChdir(modules[itteration]);
            result = ExecuteGnuMake();
            if ((module_conditions & 1) == 0)
            {
                if (result)
                    break;
            }
        }
    }
    return result;
}