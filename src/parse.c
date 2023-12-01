#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse.h"

bool iscysm(int param_1) {
    int iVar1;
    bool local_d;

    iVar1 = isalnum(param_1);
    local_d = iVar1 != 0 || param_1 == 95;
    return local_d;
}

bool getToken(
    char * param_1,
    char ** param_2
) {
    char * pcVar1;
    int iVar2;
    bool bVar3;
    char * local_10;

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
        *param_2++;
    }

    if (**param_2 == '\"') {
        *param_2++;
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
        *param_2++;
    }
    else {
        iVar2 = iscysm((int)**param_2);
        local_10 = param_1;
        if (iVar2 == 0) {
            while ( true ) {
                bVar3 = false;
                if (**param_2 != '\0') {
                    iVar2 = iscysm((int)**param_2);
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
                    iVar2 = iscysm((int)**param_2);
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