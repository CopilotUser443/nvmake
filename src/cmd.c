#include "cmd.h"

void ShowUsage (
    const char * nvmake_ver
) {
    int i;
    int aliasItteration;
    char cmdnameBuf[72];

    i = 0;
    printf("-----------------------------| nvmake version %s |--------------------------\n", nvmake_ver);
    while ( i < Global.commandCount )
    {
        cmdnameBuf[0] = '\0';
        strcat(cmdnameBuf, Global.commands[i]);
        if ( Global.aliascount[i] )
        {
            aliasItteration = 0;
            strcat(cmdnameBuf, "(");
            while ( aliasItteration < Global.aliascount[i] )
            {
                strcat(cmdnameBuf, Global.aliases[i][aliasItteration]);
                if ( aliasItteration != Global.aliascount[i] - 1)
                    strcat(cmdnameBuf, " ");
                aliasItteration++;
            }
            strcat(cmdnameBuf, ")");
        }
        printf("  %-20s %s\n", cmdnameBuf, Global.commanddescription[i]);
        i++;
    }
    printf("  help                 Help for nvmake.exe (this page)                         \n");
    printf("  verbose[=n]          Increase detail during build (default=off)              \n");
    printf("  v                    Synonym for verbose=1                                   \n");
    printf("  -time                Enables a timer showing the time of each call to gmake  \n");
    printf("\n");
    printf("Arguments containing '=' (assignments) and starting with '-' (make flags) are  \n");
    printf("passed directly to make.  Arguments starting with '@' (special make targets)   \n");
    printf("are passed directly to make after stripping off the '@'.                       \n");
    printf("\n");
    printf(" Notes: - multiple modules can be specified and are built in order             \n");
    printf("====  See %s for more information. ==== \n", engwiki);
    printf("------------------------------------------------------------------------------\n");
    return;
}