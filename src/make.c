#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "make.h"
#include "defs.h"
#include "types.h"
#include "util.h"


int ExecuteGnuMake(void) {
    FILE * pFVar1;
    time_t start_time;
    time_t finish_time;
    ulong total_time;
    int local_10 = 0;
    int local_c;
    uint * last_error;

    if (launchm != 0) {
        printf("nvmake: ********** Start Timer **********\n");
        time(&start_time);
    }

    if (launchparam > 0) {
        printf("nvmake: GnuMake Path: [%s]\n", makepath);
        printf("nvmake: GnuMake Args:\n");
        for (local_c = 1; local_c < argi; local_c++) {
            printf("         [%s]\n", args[local_c]);
        }
        printf("\n");
    }

    args[argi] = 0;
    local_10 = spawn(makepath, args);

    if (local_10 == -1) {
        printf("=======================================\n");
        printf("Failed to execute gnumake\n");
        printf("  Command: %s\n", makepath);
        printf("     Args:\n");
        for (local_c = 1; local_c < argi; local_c++){
            printf("         [%s]\n", args[local_c]);
        }
        last_error = (uint *)__error();
        printf("   Status: %lu\n", (ulong)*last_error);
    }

    if (launchm != 0) {
        time(&finish_time);
        total_time = finish_time - start_time;
        printf("nvmake: ********** Elapsed Time %02ld:%02ld (%lu seconds)**********\n", (long)total_time / 60, (long)total_time % 60, total_time & -1);
    }
    return local_10;
}

int MakeAllModules(void) {
    int local_10 = 0;
    int local_c = 0;

    if (gmake_condition < 1) {
        local_c = ExecuteGnuMake();
    } else {
        for (local_10 = 0; local_10 < gmake_condition; local_10++) {
            if (launchparam > 1) {
                printf("nvmake: Making Module @ %s\n", modules[local_10]);
            }
            MyChdir(modules[local_10]);
            local_c = ExecuteGnuMake();
            if (((module_condition & 1) == 0) && (local_c != 0)) {
                return local_c;
            }
        }
    }
    return local_c;
}