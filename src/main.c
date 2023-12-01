#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "defs.h"
#include "env.h"
#include "arg.h"
#include "make.h"
#include "util.h"
#include "host.h"

int main(int argc, char **argv) { argi = argc;args = argv;

    bool bVar1;
    char * pcVar3;
    char * pcVar4;
    int iVar2;
    int local_64;
    int local_54;
    int local_4c;
    int local_2c;
    int local_20;
    int local_1c;
    int itteration;
    char * hostOS;
    char * hostArch;
    char * uVar6;
    long lVar8;
    size_t sVar5;

    local_1c = 0;
    local_2c = 0;
    memset(Global, 0, 0x7240);
    SetupEnvironment();
    ReadBuildConfigFile();
    if (makepath == 0) {
        printf("nvmake: GMAKE setting missing in build.cfg \'%s\'\n", config_file);
        exit(1);
    }
    if (make_forced_args == 0) {
        printf("nvmake: GMAKE_FORCE_ARGS setting missing in build.cfg \'%s\'\n", config_file);       
        exit(1);
    }
    AddArgument(makepath);
    AddArgument(make_forced_args);
    AddArgument("-f makefile.nvmk");
    local_20 = 1;
    do {
        if (argc <= local_20) {
            putenv("SHELL=");
            putenv("LIB=");
            putenv("INCLUDE=");

            unsetenv("MAKE");
            unsetenv("MAKEFLAGS");
            unsetenv("MFLAGS");

            if (launchparam == 0) {
                putenv("VS_UNICODE_OUTPUT=");
            }

            pcVar3 = (char *)alloc_sprintf("NV_SOURCE=%s", Global);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

            pcVar3 = (char *)alloc_sprintf("NV_COMMON=%s", nv_common);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

            pcVar3 = (char *)alloc_sprintf("NV_TOOLS=%s", nv_tools);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

            pcVar3 = (char *)alloc_sprintf("NV_GCOMP_ROOT=%s", nv_gcomp_root);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

#ifdef __APPLE__
            pcVar3 = (char *)alloc_sprintf("NV_APPLE_TOOLS=%s", nv_apple_tools);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);
#endif

            pcVar3 = (char *)alloc_sprintf("NVMAKE_VERSION=%s", nvmake_ver);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

            hostOS = GETHOSTOS();
            hostArch = GETHOSTARCH();
            if ((hostOS == (char *)0x0) || (hostArch == (char *)0x0)) {
                exit(1);
            }
            pcVar3 = (char *)alloc_sprintf("NV_HOST_OS=%s", hostOS);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);
            pcVar3 = (char *)alloc_sprintf("NV_HOST_ARCH=%s", hostArch);
            pcVar4 = strdup(pcVar3);
            putenv(pcVar4);
            free(pcVar3);

            if (launchparam < 1) {
                AddEarlyArgument("-s");
            } else {
                AddArgument("NV_VERBOSE=%d", launchparam);
            }
            if (0 < launchparam) {
                printf("-----| nvmake version %s |-----\n", nvmake_ver);
                printf("\n");
                printf("nvmake:      NV_SOURCE: [%s]\n", Global);
                printf("nvmake:      NV_COMMON: [%s]\n", nv_common);
                printf("nvmake:       NV_TOOLS: [%s]\n", nv_tools);
#ifdef __APPLE__
                printf("nvmake: NV_APPLE_TOOLS: [%s]\n", nv_apple_tools);
#endif
                printf("nvmake:  NV_GCOMP_ROOT: [%s]\n", nv_gcomp_root);
                printf("nvmake:    config file: [%s]\n", config_file);
                printf("\n");
            }
            if (((module_condition & 0x10) != 0) && ((module_condition & 8) != 0)) {
                module_condition = module_condition & 0xffffffef;
            }
            if ((module_condition & 0x10) != 0) {
                AddArgument("clean");
                if (launchparam > 0) {
                    puts("nvmake: Cleaning...");
                }
                local_1c = MakeAllModules();
                local_2c = 1;
                if ((local_1c != 0) && ((module_condition & 1) == 0)) {
                    return local_1c;
                }
                RemoveOneArgument();
            }
            if ((module_condition & 8) != 0) {
                AddArgument("clobber");
                if (launchparam > 0) {
                    puts("nvmake: Clobbering");
                }
                local_1c = MakeAllModules();
                local_2c++;
                if ((local_1c != 0) && ((module_condition & 1) == 0)) {
                    return local_1c;
                }
                RemoveOneArgument();
            }
            if ((module_condition & 4) != 0) {
                AddArgument("depend");
                if (launchparam > 0) {
                    puts("nvmake: Depending...");
                }
                local_1c = MakeAllModules();
                local_2c++;
                if ((local_1c != 0) && ((module_condition & 1) == 0)) {
                    return local_1c;
                }
                RemoveOneArgument();
            }
            if ((module_condition & 2) != 0) {
                AddArgument("build");
                if (launchparam > 0) {
                    puts("nvmake: Building...");
                }
                local_1c = MakeAllModules();
                local_2c++;
                RemoveOneArgument();
            }
            if (local_2c == 0) {
                local_1c = MakeAllModules();
            }
            return local_1c;
        }
        pcVar3 = strdup(argv[local_20]);
        if (pcVar3 == (char *)0x0) {
            printf("nvmake: failed to allocate memory\n");
            exit(1);
        }
        currdat = 1;
        iVar2 = strcmp(pcVar3, "-help");
        if ((((((iVar2 == 0) || (iVar2 = strcmp(pcVar3,"/help"), iVar2 == 0)) || (iVar2 = strcmp(pcVar3,"help"), iVar2 == 0)) || ((iVar2 = strcmp(pcVar3,"-h"), iVar2 == 0 || (iVar2 = strcmp(pcVar3,"/h"), iVar2 == 0)))) || ((iVar2 = strcmp(pcVar3,"h"), iVar2 == 0 || ((iVar2 = strcmp(pcVar3,"-?"), iVar2 == 0 || (iVar2 = strcmp(pcVar3,"/?"), iVar2 == 0)))))) || (iVar2 = strcmp(pcVar3,"?"), iVar2 == 0))
        {
            ShowUsage(nvmake_ver);
            exit(0);
        }
        iVar2 = strncmp(pcVar3, "verbose", 7);
        if (iVar2 == 0) {
            pcVar4 = strchr(pcVar3, '=');
            if (pcVar4 == (char *)0x0) {
                local_64 = 1;
            } else {
                local_64 = atoi(pcVar4 + 1);
            }
            launchparam = local_64;
        } else {
            iVar2 = strcmp(pcVar3, "v");
            if (iVar2 == 0) {
                launchparam = 1;
            } else {
                pcVar4 = strchr(pcVar3, '=');
                if (pcVar4 == (char *)0x0) {
                    iVar2 = strncmp(pcVar3, "-time", 5);
                    if (iVar2 == 0) {
                        launchm = 1;
                    } else if (*pcVar3 == '-') {
                        AddLiteralArgument(pcVar3);
                    } else if (*pcVar3 == '@') {
                        AddLiteralArgument(pcVar3 + 1);
                    } else {
                        sVar5 = strlen(pcVar3);
                        if (pcVar3[sVar5 - 1] == '+') {
                            currdat = 1;
                            sVar5 = strlen(pcVar3);
                            pcVar3[sVar5 - 1] = '\0';
                        } else if (pcVar3[sVar5 - 1] == '-') {
                            currdat = 0;
                            sVar5 = strlen(pcVar3);
                            pcVar3[sVar5 - 1] = '\0';
                        }
                        for (itteration = 0; itteration < ccommand_count; itteration++) {
                            bVar1 = false;
                            iVar2 = strcmp(pcVar3, ccommands[itteration]);
                            if (iVar2 == 0) {
                                bVar1 = true;
                            } else {
                                for (local_54 = 0; local_54 < aliases[itteration]; local_54++) {
                                    iVar2 = strcmp(pcVar3,*(char **)((long)itteration * 0x50 + 0x10000b3a0 + (long)local_54 * 8));
                                    if (iVar2 == 0) {
                                        bVar1 = true;
                                        break;
                                    }
                                }
                            }
                            if (bVar1) {
                                iVar2 = strncmp(aliases[itteration], "module", 7);
                                if (iVar2 == 0) {
                                    if (gmake_condition > 63) {
                                        printf("nvmake: can only handle %d modules, needs recompile. See mlavoie\n", 64);
                                        exit(1);
                                    }
                                    pcVar4 = ExpandVars(aliases[itteration + 7]);
                                    pcVar4 = strdup(pcVar4);
                                    lVar8 = (long)gmake_condition;
                                    gmake_condition++;
                                    modules[lVar8] = pcVar4;
                                } else {
                                    iVar2 = strcmp(aliases[itteration], "nonstop");
                                    if (iVar2 == 0) {
                                        module_condition = module_condition | 1;
                                        AddArgument("-k");
                                    } else {
                                        iVar2 = strcmp(aliases[itteration], "build");
                                        if (iVar2 == 0) {
                                            module_condition = module_condition | 2;
                                        } else {
                                            iVar2 = strcmp(aliases[itteration], "depend");
                                            if (iVar2 == 0) {
                                                module_condition = module_condition | 4;
                                            } else {
                                                iVar2 = strcmp(aliases[itteration], "clobber");
                                                if (iVar2 == 0) {
                                                    module_condition = module_condition | 8;
                                                } else {
                                                    iVar2 = strcmp(aliases[itteration], "clean");
                                                    if (iVar2 == 0) {
                                                        module_condition = module_condition | 16;
                                                    } else {
                                                        uVar6 = ExpandVars(aliases[itteration]);
                                                        AddArgument(uVar6);
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                break;
                            }
                        }
                        if (local_4c == ccommand_count) {
                            printf("nvmake: unrecognized argument: %s\n", pcVar3);
                            exit(1);
                        }
                    }
                } else {
                    AddLiteralArgument(pcVar3);
                }
            }
        }
        free(pcVar3);
        local_20++;
    } while ( true );
}