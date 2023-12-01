#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "defs.h"
#include "parse.h"
#include "util.h"

void ReadBuildConfigFile(void)
{
    bool bVar4;
    int iVar1;
    int itteration;
    char * pcVar2;
    char * pcVar3;
    char * next_char;
    char local_818[1024];
    char local_418[1032];
    FILE * buildcfg;

    buildcfg = fopen(config_file, "r");
    if (buildcfg == (FILE *)0x0)
    {
        printf("nvmake: failed to open build config file: %s\n", config_file);
        exit(1);
    }
_BeginLoop:
    do
    {
        pcVar2 = fgets(local_418, 1024, buildcfg);
        if (pcVar2 == (char *)0x0)
        {
            fclose(buildcfg);
            return;
        }
        next_char = local_418;
        line++;
        while (true)
        {
            bVar4 = false;
            if (*next_char != '\0')
            {
                iVar1 = isspace((int)*next_char);
                bVar4 = iVar1 != 0;
            }
            if (!bVar4)
                break;
            next_char++;
        }
    } while ((((*next_char == '\0') || (*next_char == '\n')) || (*next_char == '\n')) || (*next_char == '#'));
    iVar1 = getToken(local_818, &next_char);
    if (iVar1 == false)
    {
        configError("expected %s", "command");
    }
    pcVar2 = strstr("WINDOWS-UNIX-MACOSX", local_818);
    if (pcVar2 != (char *)0x0)
        goto _CodeGen;
    goto _BeginParse;

_CodeGen:
#ifdef _WIN32
    iVar1 = strcmp(local_818, "WINDOWS");
#endif
#ifdef __APPLE__
    iVar1 = strcmp(local_818, "MACOSX");
#endif
#ifdef __linux__
    iVar1 = strcmp(local_818, "UNIX");
#endif
    if (iVar1 == false)
    {
        iVar1 = getToken(local_818, &next_char);
        if (iVar1 == false)
        {
            configError("expected %s", "command");
        }
_BeginParse:
        iVar1 = strcmp(local_818, "SET");
        if (iVar1 == false)
        {
            iVar1 = getToken(local_818, &next_char);
            if (iVar1 == false)
            {
                configError("expected %s", "variable name");
            }
            iVar1 = strcmp(local_818, "NV_BRANCH");
            if (iVar1 == false)
            {
                iVar1 = getToken(local_818, &next_char);
                if (iVar1 == false)
                {
                    configError("expected %s", "=");
                }
                iVar1 = strcmp(local_818, "=");
                if (iVar1 != false)
                {
                    configError("expected %s", "=");
                }
                iVar1 = getToken(local_818, &next_char);
                if (iVar1 == false)
                {
                    configError("expected %s", "string constant");
                }
                SET = strdup(local_818); // Ambiguous statement
            }
            else
            {
                iVar1 = strcmp(local_818, "GMAKE");
                if (iVar1 == false)
                {
                    iVar1 = getToken(local_818, &next_char);
                    if (iVar1 == false)
                    {
                        configError("expected %s", "=");
                    }
                    iVar1 = strcmp(local_818, "=");
                    if (iVar1 != 0)
                    {
                        configError("expected %s", "=");
                    }
                    iVar1 = getToken(local_818, &next_char);
                    if (iVar1 == 0)
                    {
                        configError("expected %s", "string constant");
                    }
                    makepath = strdup(local_818);
                    pcVar2 = (char *)ExpandVars(makepath);
                    free(makepath);
                    makepath = strdup(pcVar2);
                }
                else
                {
                    iVar1 = strcmp(local_818, "GMAKE_FORCE_ARGS");
                    if (iVar1 == false)
                    {
                        iVar1 = getToken(local_818, &next_char);
                        if (iVar1 == false)
                        {
                            configError("expected %s", "=");
                        }
                        iVar1 = strcmp(local_818, "=");
                        if (iVar1 != false)
                        {
                            configError("expected %s", "=");
                        }
                        iVar1 = getToken(local_818, &next_char);
                        if (iVar1 == false)
                        {
                            configError("expected %s", "string constant");
                        }
                        make_forced_args = strdup(local_818);
                        pcVar2 = (char *)ExpandVars(make_forced_args);
                        free(make_forced_args);
                        make_forced_args = strdup(pcVar2);
                    }
                    else
                    {
                        configError("unexpected variable in SET: %s", local_818);
                    }
                }
            }
        } else {
            iVar1 = strcmp(local_818, "MAKECMD");
            if (iVar1 == false) {
                //So if the command count exceeds 255, is this error fully ignored?
                if (ccommand_count == 255) {
                    configError("too many configurable commands (exceeded maximum of %d)", 256);
                }
                iVar1 = getToken(local_818, &next_char);
                if (iVar1 == false) {
                    configError("expected %s", "string constant");
                }
                pcVar2 = strdup(local_818);
                ccommands[ccommand_count] = pcVar2;
                iVar1 = getToken(local_818, &next_char);
                if (iVar1 == false) {
                    configError("expected %s", "string constant");
                }
                pcVar2 = strdup(local_818);
                cmodules[ccommand_count] = pcVar2;
                iVar1 = getToken(local_818, &next_char);
                if (iVar1 == false) {
                    configError("expected %s", "string constant");
                }
                pcVar2 = strdup(local_818);
                cnodes[ccommand_count] = pcVar2;
                ccommand_count++;
            } else {
                iVar1 = strcmp(local_818, "CMDALIAS");
                if (iVar1 == false) {
                    bVar4 = false;
                    iVar1 = getToken(local_818, &next_char);
                    if (iVar1 == false) {
                        configError("expected %s", "string constant");
                    }
                    pcVar2 = strdup(local_818);
                    for (itteration = 0; ccommand_count > itteration; itteration++) {
                        iVar1 = strcmp(ccommands[itteration], pcVar2);
                        if (iVar1 == false) {
                            bVar4 = true;
                            break;
                        }
                    }
                    if (!bVar4) {
                        configError("command %s is not defined so it cannot be aliased",pcVar2);
                    }
                    if (aliases[itteration] == 5) {
                        configError("too many aliases for %s",pcVar2);
                    }
                    iVar1 = getToken(local_818, &next_char);
                    if (iVar1 == false) {
                        configError("expected %s","string constant");
                    }
                    pcVar3 = strdup(local_818);
                    iVar1 = aliases[itteration];
                    aliases[itteration] = iVar1 + 1;
                    *(char **)((long)itteration * 0x50 + 0x10000b3a0 + (long)iVar1 * 8) = pcVar3;
                    free(pcVar2);
                } else {
                    iVar1 = strcmp(local_818, "DEPEND");
                    if (iVar1 != false) {
                        configError("expected SET, MAKECMD, CMDALIAS or DEPEND: got [%s]",local_818);
                    }
                }
            }
        }
    }
    goto _BeginLoop;
}

void SetupEnvironment(void) {
    bool bVar4;
    int iVar1;
    int NVSourceAvail;
    int BuildCFGAvail;
    int NVToolsAvail;
    int NVAppleToolsAvail;
    int NVGCompAvail;
    size_t cwdsize = 1024;
    size_t sVar2;
    struct stat * NVSourceStat = NULL;
    struct stat * BuildCFGStat = NULL;
    struct stat * NVCommonStat = NULL;
    struct stat * NVToolsStat = NULL;
    struct stat * NVAppleToolsStat = NULL;
    struct stat * NVGCompStat = NULL;
    char CurrentWorkingDirectory[1033];
    char local_818[1023];
    char local_c18[1024];
    char CWDFnop[1024];
    char * local_1170;
    char * local_10d8;
    char * local_1210;
    char * local_12b0;
    char * local_1350;
    char * pcVar3;
    char * NVSource = getenv("NV_SOURCE");
    char * NVTools = getenv("NV_TOOLS");
    char * NVGCompRoot = getenv("NV_GCOMP_ROOT");
#ifdef __APPLE__
    char * NVAppleTools = getenv("NV_APPLE_TOOLS");
#endif

    if (NVSource == (char *)0x0) {
        NVSourceAvail = 0;
        getcwd(CurrentWorkingDirectory + 1, cwdsize);
        NormalizeSlashes(CurrentWorkingDirectory + 1, '/');
        sVar2 = strlen(CurrentWorkingDirectory + 1);
        local_10d8 = CurrentWorkingDirectory + sVar2;
        do {
            sprintf(local_818, "%s/drivers/common/build/build.cfg", CurrentWorkingDirectory + 1);
#ifdef __APPLE__
            iVar1 = stat(local_818, NVSourceStat);
#elif _WIN32
            iVar1 = _stat(local_818, NVSourceStat);
#endif
            if (iVar1 != -1) {
                NVSourceAvail = 1;
                break;
            }
            while ( true ) {
                bVar4 = false;
                if (CurrentWorkingDirectory + 1 < local_10d8) {
                    bVar4 = *local_10d8 != '/';
                }
                if (!bVar4) break;
                local_10d8--;
            }
            if (*local_10d8 == '/') {
                *local_10d8 = '\0';
                local_10d8--;
            }
        } while (CurrentWorkingDirectory + 1 < local_10d8);
        if (NVSourceAvail == 0) {
            getcwd(CurrentWorkingDirectory + 1, 1024);
            NormalizeSlashes(CurrentWorkingDirectory + 1, '/');
            sVar2 = strlen(CurrentWorkingDirectory + 1);
            local_10d8 = CurrentWorkingDirectory + sVar2;
            do {
                sprintf("%s/common/build/build.cfg", CurrentWorkingDirectory + 1);
#ifdef __APPLE__
                iVar1 = stat(local_818, NVSourceStat);
#elif _WIN32
                iVar1 = _stat(local_818, NVSourceStat);
#endif
                if (iVar1 != -1) {
                    NVSourceAvail = 1;
                    break;
                }
                while ( true ) {
                    bVar4 = false;
                    if (CurrentWorkingDirectory + 1 < local_10d8) {
                        bVar4 = *local_10d8 != '/';
                    }
                    if (!bVar4) break;
                    local_10d8--;
                }
                if (*local_10d8 == '/') {
                    *local_10d8 = '\0';
                    local_10d8--;
                }
            } while (CurrentWorkingDirectory + 1 < local_10d8);
            if (NVSourceAvail == 0) {
                printf("nvmake: Unable to locate build.cfg.  \n");
                printf("Therefore, unable to automatically determine NV_SOURCE.\n");
                printf("        See %s for more information.\n", "https://engwiki/index.php/Nvmake");
                exit(1);
            }
            strcpy(Global, CurrentWorkingDirectory + 1);
        } else {
            strcpy(Global, CurrentWorkingDirectory + 1);
        }
    } else {
        strcpy(Global, NVSource);
    }
    NormalizeSlashes(Global, '/');
    local_1170 = (char *)0x0;
    sprintf(local_c18, "%s/drivers/common/build/build.cfg", Global);
#ifdef __APPLE__
    iVar1 = stat(local_c18, NVCommonStat);
#elif _WIN32
    iVar1 = _stat(local_c18, NVCommonStat);
#endif
    if (iVar1 != -1) {
        local_1170 = "/drivers/common";
    }
    sprintf(local_c18, "%s/common/build/build.cfg", Global);
#ifdef __APPLE__
    iVar1 = stat(local_c18, NVCommonStat);
#elif _WIN32
    iVar1 = _stat(local_c18, NVCommonStat);
#endif
    if (iVar1 != -1) {
        local_1170 = "/common";
    }
    if (local_1170 == (char *)0x0) {
        printf("nvmake: NV_SOURCE: %s\n",Global);
        printf("Unable to derive NV_COMMON from NV_SOURCE.\n");
        printf("Does %s/(drivers/)common/build/build.cfg exist?\n", Global);
        printf("        See %s for more information.\n", "https://engwiki/index.php/Nvmake");
        exit(1);
    }
    strcpy(nv_common, Global);
    strcat(nv_common, local_1170);
    NormalizeSlashes(nv_common, '/');
    BuildCFGAvail = 0;
    getcwd(CWDFnop, cwdsize);
    NormalizeSlashes(CWDFnop, '/');
    sVar2 = strlen(CWDFnop);
    local_1210 = CWDFnop + (sVar2 - 1);
    do {
        sprintf(config_file, "%s/build.cfg", CWDFnop);
#ifdef __APPLE__
        iVar1 = stat(config_file, BuildCFGStat);
#elif _WIN32
        iVar1 = _stat(config_file, BuildCFGStat);
#endif
        if (iVar1 != -1) {
            BuildCFGAvail = 1;
            break;
        }
        while ( true ) {
            bVar4 = false;
            if (CWDFnop < local_1210) {
                bVar4 = *local_1210 != '/';
            }
            if (!bVar4) break;
            local_1210--;
        }
        if (*local_1210 == '/') {
            *local_1210 = '\0';
            local_1210--;
        }
    } while (CWDFnop < local_1210);
    if (BuildCFGAvail == 0) {
        sprintf(config_file, "%s/common/build/build.cfg", Global);
#ifdef __APPLE__
        iVar1 = stat(config_file, BuildCFGStat);
#elif _WIN32
        iVar1 = _stat(config_file, BuildCFGStat);
#endif
        if (iVar1 == -1) {
            sprintf(config_file, "%s/drivers/common/build/build.cfg", Global);
#ifdef __APPLE__
            iVar1 = stat(config_file, BuildCFGStat);
#elif _WIN32
            iVar1 = _stat(config_file, BuildCFGStat);
#endif
            if (iVar1 != -1) {
                BuildCFGAvail = 1;
            }
        } else {
            BuildCFGAvail = 1;
        }
        if (BuildCFGAvail == 0) {
            printf("nvmake: Unable to locate build.cfg file.\n");
            printf("        See %s for more information.\n", "https://engwiki/index.php/Nvmake");
            exit(1);
        }
    }
    if (NVTools == (char *)0x0) {
        NVToolsAvail = 0;
        strcpy(nv_tools, Global);
        NormalizeSlashes(nv_tools, '/');
        do {
            local_12b0 = strrchr(nv_tools, '/');
            if (local_12b0 == (char *)0x0) break;
            strcpy(local_12b0 + 1, "tools");
            local_12b0[6] = '\0';
#ifdef __APPLE__
            iVar1 = stat(nv_tools, NVToolsStat);
#elif _WIN32
            iVar1 = _stat(nv_tools, NVToolsStat);
#endif
            if (iVar1 == -1) {
                *local_12b0 = '\0';
            } else {
                NVToolsAvail = 1;
            }
        } while (local_12b0 == 0);
        if (NVToolsAvail == 0) {
            printf("nvmake: NV_SOURCE: %s\n", Global);
            printf("Unable to derive NV_TOOLS from NV_SOURCE.\n");
            printf("        Please set NV_TOOLS.\n");
            printf("        See %s for more information.\n", "https://engwiki/index.php/Nvmake");
            exit(1);
        }
    } else {
        strcpy(nv_tools, NVTools);
        NormalizeSlashes(nv_tools, '/');
    }
#ifdef __APPLE__
    if (NVAppleTools == (char *)0x0) {
        NVAppleToolsAvail = 0;
        strcpy(nv_apple_tools, Global);
        NormalizeSlashes(nv_apple_tools, '/');
        do {
            local_1350 = strrchr(nv_apple_tools, '/');
            if (local_1350 == (char *)0x0) break;
            strcpy(local_1350 + 1, "apple/tools");
            local_1350[12] = '\0';
            iVar1 = stat(nv_apple_tools, NVAppleToolsStat);
            if (iVar1 == -1) {
                *local_1350 = '\0';
            } else {
                NVAppleToolsAvail = 1;
            }
        } while (NVAppleToolsAvail == 0);
        if (NVAppleToolsAvail == 0) {
            strcpy(nv_apple_tools, "<NV_APPLE_TOOLS_NOT_FOUND>");
        }
    } else {
        strcpy(nv_apple_tools, NVAppleTools);
        NormalizeSlashes(nv_apple_tools, '/');
    }
#endif
    if (NVGCompRoot == (char *)0x0) {
        NVGCompAvail = 0;
        strcpy(nv_gcomp_root, nv_tools);
        NormalizeSlashes(nv_gcomp_root, '/');
        pcVar3 = strstr(nv_gcomp_root, "tools");
        if (pcVar3 != (char *)0x0) {
            strcpy(pcVar3, "gcomp");
            pcVar3[5] = '\0';
#ifdef __APPLE__
            iVar1 = stat(nv_gcomp_root, NVGCompStat);
#elif _WIN32
            iVar1 = _stat(nv_gcomp_root, NVGCompStat);
#endif
            if (iVar1 != -1) {
                NVGCompAvail = 1;
            }
        }
        if (NVGCompAvail == 0) {
            getcwd(nv_gcomp_root, cwdsize);
            NormalizeSlashes(nv_gcomp_root, '/');
            do {
                pcVar3 = strrchr(nv_gcomp_root, '/');
                if (pcVar3 == (char *)0x0) break;
                strcpy(pcVar3 + 1, "gcomp");
                pcVar3[5] = '\0';
#ifdef __APPLE__
                iVar1 = stat(nv_gcomp_root, NVGCompStat);
#elif _WIN32
                iVar1 = _stat(nv_gcomp_root, NVGCompStat);
#endif
                if (iVar1 == -1) {
                    *pcVar3 = '\0';
                } else {
                    NVGCompAvail = 1;
                }
            } while (NVGCompAvail == 0);
        }
        if ((NVGCompAvail == 0) && (strcpy(nv_gcomp_root, "<gCompDirNotFound>"), launchparam > 0)) {
            printf("nvmake: NV_SOURCE: %s\n", Global);
            printf("        NV_COMMON: %s\n", nv_common);
            printf("         NV_TOOLS: %s\n", nv_tools);
            printf("        Unable to derive NV_GCOMP_ROOT from NV_TOOLS or NV_SOURCE.\n");
            printf("         NV_GCOMP_ROOT set to: %s\n", nv_gcomp_root);
        }
    } else {
        strcpy(nv_gcomp_root, NVGCompRoot);
        NormalizeSlashes(nv_gcomp_root, '/');
    }
    return;
}

void ShowUsage(const char * nvmake_version) {
    int first_itteration;
    int second_itteration;
    char local_58[72];

    printf("-----------------------------| nvmake version %s |--------------------------\n", nvmake_version);
    for (first_itteration = 0; first_itteration < ccommand_count; first_itteration++) {
        local_58[0] = 0;
        strcat(local_58, ccommands[first_itteration]);
        if (aliases[first_itteration] != 0) {
            strcat(local_58, "(");
            for (second_itteration = 0; second_itteration < aliases[first_itteration]; second_itteration++) {
                strcat(local_58, *(char **)((long)first_itteration * 0x50 + 0x10000b3a0 + (long)second_itteration * 8));
                if (second_itteration != aliases[first_itteration - 1]) {
                    strcat(local_58, " ");
                }
            }
            strcat(local_58, ")");
        }
        printf("  %-20s %s\n", local_58, cnodes[first_itteration]);
    }
    printf("  help                 Help for nvmake.exe (this page)                         \n");
    printf("  verbose[=n]          Increase detail during build (default=off)              \n");
    printf("  v                    Synonym for verbose=1                                   \n");
    printf("  -time                Enables a timer showing the time of each call to gmake  \n");
    printf("\n");
    printf("Arguments containing \'=\' (assignments) and starting with \'-\' (make flags) are  \n");
    printf("passed directly to make.  Arguments starting with \'@\' (special make targets)   \n");
    printf("are passed directly to make after stripping off the \'@\'.                       \n");
    printf("\n");
    printf(" Notes: - multiple modules can be specified and are built in order             \n");
    printf("====  See %s for more information. ==== \n", "https://engwiki/index.php/Nvmake");
    printf("------------------------------------------------------------------------------\n");
    return;
}