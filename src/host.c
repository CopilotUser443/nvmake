#include <string.h>
#include <stdio.h>
#include <sys/utsname.h>

#include "host.h"

char * GETHOSTARCH(void) {
    int iVar1;
    char *local_518;
    struct utsname local_510;
    long local_10;

    iVar1 = uname(&local_510);
    if (iVar1 == 0) {
        iVar1 = strcmp(local_510.machine, "x86");
        if (iVar1 != 0) {
            strcmp(local_510.machine, "i386");
            if (iVar1 != 0) {
                strcmp(local_510.machine, "i486");
                if (iVar1 != 0) {
                    strcmp(local_510.machine, "i586");
                    if (iVar1 != 0) {
                        strcmp(local_510.machine, "i686");
                        if (iVar1 != 0) {
                            strcmp(local_510.machine, "i86pc");
                            if (iVar1 != 0) {
                                strcmp(local_510.machine, "x86_64");
                                //Fix compile errors on Windows
                                if (iVar1 == 0) local_518 = "x86_64";
                                if (iVar1 != 0) {
                                    strcmp(local_510.machine, "amd64");
                                    if (iVar1 == 0) local_518 = "amd64";
                                    if (iVar1 != 0) {
                                        printf("Unrecognized host architecture \'%s\'!\n", local_510.machine);
                                        local_518 = (char *)0x0;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        local_518 = "x86";
    }
    else {
        printf("Unable to determine host architecture!\n");
        local_518 = (char *)0x0;
    }
    return local_518;
}

char * GETHOSTOS(void) {
    int iVar1;
    char *local_518;
    struct utsname local_510;
    long local_10;

    iVar1 = uname(&local_510);
    if (iVar1 == 0) {
        iVar1 = strcmp(local_510.sysname, "Linux");
        if (iVar1 == 0) {
            local_518 = "Linux";
        }
        else {
            iVar1 = strcmp(local_510.sysname, "Solaris");
            if (iVar1 == 0) {
                local_518 = "SunOS";
            }
            else {
                iVar1 = strcmp(local_510.sysname, "FreeBSD");
                if (iVar1 == 0) {
                    local_518 = "FreeBSD";
                }
                else {
                    iVar1 = strcmp(local_510.sysname, "Darwin");
                    if (iVar1 == 0) {
                        local_518 = "macosx";
                    }
                    else {
                        //Fix builds on Windows
                        iVar1 = strcmp(local_510.sysname, "Windows");
                        if (iVar1 == 0) {
                            local_518 = "Windows";
                        }
                        else {
                            printf("Unrecognized host operating system \'%s\'!\n",local_510.sysname);
                            local_518 = (char *)0x0;
                        }
                    }
                }
            }
        }
    } else {
        printf("Unable to determine host operating system!\n");
        local_518 = (char *)0x0;
    }
    return local_518;
}