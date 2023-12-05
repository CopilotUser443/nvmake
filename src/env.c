#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>

#ifdef _WIN32
#include "unistd.w.h"

#elif defined __unix__
#include <unistd.h>
#define _stat stat

#endif


#include "util.h"
#include "var.h"

extern int debug;
extern int line;
extern int ccommands_count;
extern char * nv_source;
extern char * nv_tools;
extern char * nv_apple_tools;
extern char * nv_gcomp_root;
extern char * nv_common;
extern char * config_path;
extern char * SET;
extern char * makepath;
extern char * make_force_args;
extern const char * nvmake_wiki;
extern char ccommands[1024][20];
extern char cmodules[1024][20];
extern char cnodes[1024][20]; 
extern char aliases[1024][20];
extern char unknown[1024][20];

void SetupEnvironment()
{
    int stat_ret;
    int cfg_file_avail;
    int cfg_code;
    int nv_common_avail;
    int nv_common_code;
    int nv_tools_avail;
    int nv_gcomp_root_avail;

    char * nvstr;
    char * nvstr_2;
    char * nvstr_3;
    char * nvstr_4;
    char * nvcommonfs;
    char * NVSource = (char*) malloc(1024 * sizeof(char));
    char * NVTools = (char*) malloc(1024 * sizeof(char));
    char * NVGCompRoot = (char*) malloc(1024 * sizeof(char));

    nv_source = (char*) malloc(1024 * sizeof(char));
    nv_tools = (char*) malloc(1024 * sizeof(char));
    nv_apple_tools = (char*) malloc(1024 * sizeof(char));
    nv_gcomp_root = (char*) malloc(1024 * sizeof(char));
    nv_common = (char*) malloc(1024 * sizeof(char));
    config_path = (char*) malloc(1024 * sizeof(char));
    SET = (char*) malloc(1024 * sizeof(char));
    makepath = (char*) malloc(1024 * sizeof(char));
    make_force_args = (char*) malloc(1024 * sizeof(char));

#ifdef __APPLE__
    int nv_apple_tools_avail;
    char * nvapplestr;
    char * NVAppleTools = getenv("NV_APPLE_TOOLS");
#endif

    char cfgfile[2048];
    char nv_source_path[2048];
    char cwd[1032];
    char common_cwd[1032];
    char * nv_source_fs;
    char * cfgfs;
    char * nv_tools_fs;
    char * nv_gcomp_root_fs;
    struct _stat cfgstat;
    struct _stat nv_source_stat;
    struct _stat config_stat;
    struct _stat nv_tools_stat;
    struct _stat nv_apple_tools_stat;
    struct _stat nv_gcomp_root_stat;

#ifdef __unix__
    NVSource    = getenv("NV_SOURCE");
    NVTools     = getenv("NV_TOOLS");
    NVGCompRoot = getenv("NV_GCOMP_ROOT"); 
#endif
#ifdef _WIN32
    size_t foundenv;

    getenv_s(&foundenv, NULL, 0, "NV_SOURCE");
    if (foundenv == 0) NVSource = '\0';
    else getenv_s(&foundenv, NVSource, sizeof NVSource, "NV_SOURCE");

    getenv_s(&foundenv, NULL, 0, "NV_TOOLS");
    if (foundenv == 0) NVTools = '\0';
    else getenv_s(&foundenv, NVTools, sizeof NVTools, "NV_TOOLS");

    getenv_s(&foundenv, NULL, 0, "NV_GCOMP_ROOT");
    if (foundenv == 0) NVGCompRoot = '\0';
    else getenv_s(&foundenv, NVGCompRoot, sizeof NVGCompRoot, "NV_GCOMP_ROOT");
#endif

    if (NVSource)
    {
        strcpy(nv_source, NVSource);
    }
    else
    {
        cfg_file_avail = 0;
        getcwd(cwd, 0x400uLL);
        NormalizeSlashes(cwd, '/');
        cfgfs = &cwd[strlen(cwd) - 1];
        while (true)
        {
            sprintf(cfgfile, "%s/drivers/common/build/build.cfg", cwd);
            if (_stat(cfgfile, &cfgstat) != -1) break;
            while (true)
            {
                stat_ret = 0;
                if (cfgfs > cwd)
                {
                    stat_ret = *cfgfs != '/';
                }
                if (!stat_ret) break;
                cfgfs--;
            }
            if (*cfgfs == '/')
            {
                nvstr = cfgfs--;
                *nvstr = 0;
            }
            if ( cfgfs <= cwd)
            {
                goto _BeginLoop;
            }
        }
        cfg_file_avail = 1;
_BeginLoop:
        if ( !cfg_file_avail )
        {
            getcwd(cwd, 0x400uLL);
            NormalizeSlashes(cwd, '/');
            cfgfs = &cwd[strlen(cwd) - 1];
            while(true)
            {
                sprintf(cfgfile, "%s/common/build/build.cfg", cwd);
                if (_stat(cfgfile, &cfgstat) != -1) break;
                while (true)
                {
                    cfg_code = 0;
                    if (cfgfs > cwd)
                    {
                        cfg_code = *cfgfs != '/';
                    }
                    if (!cfg_code) break;
                    cfgfs--;
                }
                if (*cfgfs == '/')
                {
                    nvstr_2 = cfgfs--;
                    *nvstr_2 = 0;
                }
                if (cfgfs <= cwd) 
                {
                    goto _ParseCodes;
                }
            }
            cfg_file_avail = 1;
_ParseCodes:
            if (!cfg_file_avail)
            {
                fprintf(stderr, "nvmake: Unable to locate build.cfg.  \n");
                fprintf(stderr, "        Therefore, unable to automatically determine NV_SOURCE.\n");
                fprintf(stderr, "        See %s for more information.\n", nvmake_wiki);
                exit(1);
            }
        }
        strcpy(nv_source, cwd);
    }
    NormalizeSlashes(nv_source, '/');
    nv_source_fs = '\0';
    sprintf(nv_source_path, "%s/drivers/common/build/build.cfg", nv_source);
    if (_stat(nv_source_path, &nv_source_stat) != -1)
    {
        nv_source_fs = "/drivers/common";
    }
    sprintf(nv_source_path, "%s/common/build/build.cfg", nv_source);
    if (_stat(nv_source_path, &nv_source_stat) != -1)
    {
        nv_source_fs = "/common";
    }
    if (!nv_source_fs)
    {
        fprintf(stderr, "nvmake: NV_SOURCE: %s\n", nv_source);
        fprintf(stderr, "        Unable to derive NV_COMMON from NV_SOURCE.\n");
        fprintf(stderr, "        Does %s/(drivers/)common/build/build.cfg exist?\n", nv_source);
        fprintf(stderr, "        See %s for more information.\n", nvmake_wiki);
        exit(1);
    }
    strcpy(nv_common, nv_source);
    strcat(nv_common, nv_source_fs);
    NormalizeSlashes(nv_common, '/');
    nv_common_avail = 0;
    getcwd(common_cwd, 0x400uLL);
    NormalizeSlashes(common_cwd, '/');
    nvcommonfs = &common_cwd[strlen(common_cwd) - 1];
    while (true)
    {
        sprintf(config_path, "%s/build.cfg", common_cwd);
        if (_stat(config_path, &config_stat) != -1) break;
        while (true)
        {
            nv_common_code = 0;
            if (nvcommonfs > common_cwd)
                nv_common_code = *nvcommonfs != '/';
            if (!nv_common_code) break;
            nvcommonfs--;
        }
        if (*nvcommonfs == '/')
        {
            nvstr_3 = nvcommonfs--;
            *nvstr_3 = 0;
        }
        if (nvcommonfs <= common_cwd) {
            goto _ParseConfig;
        }
    }
    nv_common_avail = 1;
_ParseConfig:
    if (!nv_common_avail)
    {
        sprintf(config_path, "%s/common/build/build.cfg", nv_source);
        if (_stat(config_path, &config_stat) == -1)
        {
            sprintf(config_path, "%s/drivers/common/build/build.cfg", nv_source);
            if (_stat(config_path, &config_stat) != -1)
            {
                nv_common_avail = 1;
            }
        }
        else
        {
            nv_common_avail = 1;
        }
        if (!nv_common_avail)
        {
            fprintf(stderr, "nvmake: Unable to locate build.cfg file.\n");
            fprintf(stderr, "        See %s for more information.\n", nvmake_wiki);
            exit(1);
        }
    }
    if (NVTools)
    {
        strcpy(nv_tools, NVTools);
        NormalizeSlashes(nv_tools, '/');
    }
    else
    {
        nv_tools_avail = 0;
        strcpy(nv_tools, nv_source);
        NormalizeSlashes(nv_tools, '/');
        do
        {
            nv_tools_fs = strrchr(nv_tools, '/');
            if (!nv_tools_fs) break;
            strcpy(nv_tools_fs + 1, "tools");
            nv_tools_fs[6] = '\0';
            if (_stat(nv_tools, &nv_tools_stat) == -1)
            {
                *nv_tools_fs = 0;
            }
            else
            {
                nv_tools_avail = 1;
            }
        } while ( !nv_tools_avail );
        if (!nv_tools_avail)
        {
            fprintf(stderr, "nvmake: NV_SOURCE: %s\n", nv_source);
            fprintf(stderr, "        Unable to derive NV_TOOLS from NV_SOURCE.\n");
            fprintf(stderr, "        Please set NV_TOOLS.\n");
            fprintf(stderr, "        See %s for more information.\n", nvmake_wiki);
            exit(1);
        }
    }
#ifdef __APPLE__
    if (NVAppleTools)
    {
        strcpy(nv_apple_tools, NVAppleTools);
        NormalizeSlashes(nv_apple_tools, '/');
    }
    else
    {
        nv_apple_tools_avail = 0;
        strcpy(nv_apple_tools, nv_source);
        NormalizeSlashes(nv_apple_tools, '/');
        do
        {
            nvapplestr = strrchr(nv_apple_tools, '/');
            if (!nvapplestr) break;
            strcpy(nvapplestr + 1, "apple/tools");
            nvapplestr[12] = 0;
            if (_stat(nvapplestr, &nv_apple_tools_stat) == -1)
            {
                *nvapplestr = 0;
            }
            else
            {
                nv_apple_tools_avail = 1;
            }
        } while (!nv_apple_tools_avail);
        if (!nv_apple_tools) {
            strcpy(nv_apple_tools, "<NV_APPLE_TOOLS_NOT_FOUND>");
        }        
    }
#endif
    if (NVGCompRoot)
    {
        strcpy(nv_gcomp_root, NVGCompRoot);
        NormalizeSlashes(nv_gcomp_root, '/');
    }
    else
    {
        nv_gcomp_root_avail = 0;
        strcpy(nv_gcomp_root, nv_source);
        NormalizeSlashes(nv_gcomp_root, '/');
        nvstr_4 = strstr(nv_gcomp_root, "tools");
        if (nvstr_4)
        {
            strcpy(nvstr_4, "gcomp");
            nvstr_4[5] = 0;
            if (_stat(nv_gcomp_root, &nv_apple_tools_stat) != -1)
            {
                nv_gcomp_root_avail = 1;
            }
        }
        if (!nv_gcomp_root_avail)
        {
            getcwd(nv_gcomp_root, 0x400uLL);
            NormalizeSlashes(nv_gcomp_root, '/');
            do
            {
                nv_gcomp_root_fs = strrchr(nv_gcomp_root, '/');
                if (!nv_gcomp_root_fs) break;
                strcpy(nv_gcomp_root_fs + 1, "gcomp");
                nv_gcomp_root_fs[5] = 0;
                if (_stat(nv_gcomp_root, &nv_gcomp_root_stat) == -1)
                {
                    *nv_gcomp_root_fs = '\0';
                }
                else
                {
                    nv_gcomp_root_avail = 1;
                }
            } while (!nv_gcomp_root_avail);
        }
        if (!nv_gcomp_root_avail)
        {
            strcpy(nv_gcomp_root, "<gCompDirNotFound>");
            if (debug > 0)
            {
                fprintf(stderr, "nvmake: NV_SOURCE: %s\n", nv_source);
                fprintf(stderr, "        NV_COMMON: %s\n", nv_common);
                fprintf(stderr, "         NV_TOOLS: %s\n", nv_tools);
                fprintf(stderr, "        Unable to derive NV_GCOMP_ROOT from NV_TOOLS or NV_SOURCE.\n");
                fprintf(stderr, "         NV_GCOMP_ROOT set to: %s\n", nv_gcomp_root);
            }
        }
    }
    return;
}

int ReadBuildConfigFile()
{
    bool cfgcondition;
    int tempint;
    int itteration;
    unsigned int * v1;
    unsigned int v2;
    FILE * cfgfile;
    char * tmpstring;
    char * tmpstring_2;
    char next_token[1024];
    char buffer[1032];
    char * citteration;
    const char * compile_platform;

#ifdef _WIN32
    compile_platform = "WINDOWS";
#elif defined(__linux__)
    compile_platform = "UNIX";
#elif defined(_APPLE__)
    compile_platform = "MACOSX";
#endif

    cfgfile = fopen(config_path, "r");
    if (!cfgfile)
    {
        fprintf(stderr, "nvmake: failed to open build config file: %s\n", config_path);
        exit(1);
    }
    while (fgets(buffer, 1024, cfgfile))
    {
        line++;
        for (citteration = buffer; ; citteration++)
        {
            cfgcondition = 0;
            if (*citteration)
            {
                cfgcondition = isspace(*citteration) != 0;
            }
            if (!cfgcondition) break;
        }
        if (*citteration != '\0' && *citteration != '\n' && *citteration != '#')
        {
            if (!getToken(next_token, &citteration))
            {
                configError("expected %s", "command");
            }
            if (!strstr("WINDOWS-UNIX-MACOSX", next_token))
            {
                goto _ReadCFG;
            }
            if (!strcmp(next_token, compile_platform))
            {
                if (!getToken(next_token, &citteration))
                {
                    configError("expected %s", "command");
                }
_ReadCFG:       
                if (!strcmp(next_token, "SET"))
                {
                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "variable name");
                    }
                    if (!strcmp(next_token, "NV_BRANCH"))
                    {
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "=");
                        }
                        if (strcmp(next_token, "="))
                        {
                            configError("expected %s", "=");
                        }
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "string constant");
                        }
                        SET = strdup(next_token);
                    }
                    else if (!strcmp(next_token, "GMAKE"))
                    {
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "=");
                        }
                        if (strcmp(next_token, "="))
                        {
                            configError("expected %s", "=");
                        }
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "string constant");
                        }
                        makepath = strdup(next_token);
                        tmpstring = (char *)ExpandVars(makepath);
                        free((void *)makepath);
                        makepath = strdup(tmpstring);
                    }
                    else
                    {
                        if (strcmp(next_token, "GMAKE_FORCE_ARGS"))
                        {
                            configError("unexpected variable in SET: %s", next_token);
                        }
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "=");
                        }
                        if (strcmp(next_token, "="))
                        {
                            configError("expected %s", "=");
                        }
                        if (!getToken(next_token, &citteration))
                        {
                            configError("expected %s", "string constant");
                        }
                        make_force_args = strdup(next_token);
                        tmpstring = (char *)ExpandVars(make_force_args);
                        free((void *)make_force_args);
                        make_force_args = strdup(tmpstring);
                    }
                }
                else if (!strcmp(next_token, "MAKECMD"))
                {
                    if(ccommands_count == 255)
                    {
                        configError("too many configurable commands (exceeded maximum of %d)", 256);
                    }
                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "string constant");
                    }
                    strcpy(ccommands[ccommands_count], strdup(next_token));

                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "string constant");
                    }
                    strcpy(cmodules[ccommands_count], strdup(next_token));

                    
                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "string constant");
                    }
                    strcpy(cnodes[ccommands_count], strdup(next_token));
                    ccommands_count++;
                }
                else if (!strcmp(next_token, "CMDALIAS"))
                {
                    tempint = 0;
                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "string constant");
                    }
                    tmpstring = strdup(next_token);
                    for (itteration = 0; itteration < ccommands_count; itteration++)
                    {
                        if (!strcmp(ccommands[itteration], tmpstring))
                        {
                            tempint = 1;
                            break;
                        }
                    }
                    if (!tempint)
                    {
                        configError("command %s is not defined so it cannot be aliased", tmpstring);
                    }
                    if (aliases[itteration] == 5)
                    {
                        configError("too many aliases for %s", tmpstring);
                    }
                    if (!getToken(next_token, &citteration))
                    {
                        configError("expected %s", "string constant");
                    }
                    tmpstring_2 = strdup(next_token);
                    v1 = ccommands[itteration];
                    v2 = v1[14];
                    v1[14] = v2 + 1;
                    strcpy(unknown[v2 + 1], tmpstring_2);
                    free(tmpstring);
                }
                else if (strcmp(next_token, "DEPEND"))
                {
                    configError("expected SET, MAKECMD, CMDALIAS or DEPEND: got [%s]", next_token);
                }
            }
        }
    }
    return fclose(cfgfile);
}





























