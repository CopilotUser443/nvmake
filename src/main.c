#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __unix__
#define _putenv putenv
#endif

#include "vars.h"
#include "env.h"
#include "arg.h"
#include "util.h"
#include "var.h"
#include "make.h"
#include "host.h"

int main(int argc, const char ** argv, const char ** envp) {
    argi = argc;
    args = argv;
    envs = envp;

    int argument_counter;
    int itteration;
    int second_itteration;
    int third_itteration;
    int debug_enabled;
    int ccommand_avail;
    int module_ccount;
    int conditional_statement;
    int temporal_condition;
    int AllModules;
    int recursive_itteration = 0;
    char firstarg_char;
    char * debug_str;
    char  *firstarg = (char*) malloc(1024 * sizeof(char));
    char * tmpstring_1;
    char * tmpstring_2;
    char * tmpstring_3;
    char * tmpstring_4;
    char * tmpstring_5;
    char * tmpstring_6;
    char * tmpstring_7;
    char * tmpstring_8;
    char * hostos;
    char * hostarch;
    char * delstr;
    char * dupstr;
    char * compstr;
    const char * tmpchar;

    SetupEnvironment();
    ReadBuildConfigFile();
    if (!makepath)
    {
        fprintf(stderr, "nvmake: GMAKE setting missing in build.cfg '%s'\n", config_path);
        exit(1);
    }
    if (!make_force_args)
    {
        fprintf(stderr, "nvmake: GMAKE_FORCE_ARGS setting missing in build.cfg \'%s\'\n", config_path);
        exit(1);
    }

    AddArgument(makepath);
    AddArgument(make_force_args);
    AddArgument("-f makefile.nvmk");
    for (itteration = 1; itteration < argc; itteration++)
    {
        strcpy(firstarg, strdup(args[itteration]));
        if (!firstarg)
        {
            fprintf(stderr, "nvmake: failed to allocate memory\n");
            exit(1);
        }
        nv_opt_state = 1;
        if ( !strcmp(firstarg, "-help")
          || !strcmp(firstarg, "/help")
          || !strcmp(firstarg, "help")
          || !strcmp(firstarg, "-h")
          || !strcmp(firstarg, "/h")
          || !strcmp(firstarg, "h")
          || !strcmp(firstarg, "-?")
          || !strcmp(firstarg, "/?")
          || !strcmp(firstarg, "?") )
        {
            ShowUsage("2.07");
            exit(0);
        }
        if (!strncmp(firstarg, "verbose", 7uLL))
        {
            debug_str = strchr(firstarg, '=');
            if (debug_str)
            {
                debug_enabled = atoi(debug_str + 1);
            } 
            else
            {
                debug_enabled = 1;
            }
            debug = debug_enabled;
        }
        else
        {
            if (!strcmp(firstarg, "v"))
            {
                debug = 1;
                goto _StartEnv;
            }
            if (strchr(firstarg, '='))
            {
_ParseArg:
                AddLiteralArgument(firstarg);
                goto _StartEnv;
            }
            if (!strncmp(firstarg, "-time", 5uLL))
            {
                launchm = 1;
            }
            else
            {
                if (*firstarg == '-')
                    goto _ParseArg;
                if (*firstarg == '@')
                {
                    AddLiteralArgument(firstarg + 1);
                }
                else
                {
                    firstarg_char = firstarg[strlen(firstarg) - 1];
                    if (firstarg_char == '+')
                    {
                        nv_opt_state = 1;
                        firstarg[strlen(firstarg) - 1] = 0;
                    }
                    for (second_itteration = 0; second_itteration < ccommands_count; second_itteration++)
                    {
                        ccommand_avail = 0;
                        if (!strcmp(firstarg, ccommands[second_itteration]))
                        {
                            ccommand_avail = 1;
                        }
                        else
                        {
                            for (third_itteration = 0; third_itteration < aliases[second_itteration]; third_itteration++)
                            {
                                if (!strcmp(firstarg, unknown[third_itteration]))
                                {
                                    ccommand_avail = 1;
                                    break;
                                }
                            }
                        }
                        if (ccommand_avail)
                        {
                            if (!strncmp(cmodules[second_itteration], "module ", 7uLL))
                            {
                                if (make_conditions >= 64)
                                {
                                    fprintf(stderr, "nvmake: can only handle %d modules, needs recompile, See mlavoie\n", 64);
                                    exit(1);
                                }
                                tmpchar = (const char *)ExpandVars(cmodules[second_itteration] + 7LL);
                                dupstr = strdup(tmpchar);
                                module_ccount = make_conditions;
                                make_conditions = module_ccount + 1;
                                modules[module_ccount] = dupstr; 
                            }
                            else if (!strcmp(cmodules[second_itteration], "nonstop"))
                            {
                                conditional_statement = module_conditions | 1;
                                module_conditions = conditional_statement;
                                AddArgument("-k");
                            }
                            else if (!strcmp(cmodules[second_itteration], "build"))
                            {
                                module_conditions |= 2;
                            }
                            else if (!strcmp(cmodules[second_itteration], "depend"))
                            {
                                module_conditions |= 4;
                            }
                            else if (!strcmp(cmodules[second_itteration], "clobber"))
                            {
                                module_conditions |= 8;
                            }
                            else if (!strcmp(cmodules[second_itteration], "clean"))
                            {
                                module_conditions |= 16;
                            }
                            else
                            {
                                temporal_condition = ExpandVars(cmodules[second_itteration]);
                                AddArgument(temporal_condition, "clean");
                            }
                            break;
                        }
                    }
                    if (second_itteration == ccommands_count)
                    {
                        fprintf(stderr, "nvmake: unrecognized argument: %s\n", firstarg);
                        exit(1);
                    }
                }
            }
        }
_StartEnv:
    }
    _putenv("SHELL=");
    _putenv("LIB=");
    _putenv("INCLUDE=");
    _putenv("MAKE=");
    _putenv("MAKEFLAGS=");
    _putenv("MFLAGS=");
    if (!debug)
    {
        _putenv("VS_UNICODE_OUTPUT=");
    }
    tmpstring_1 = (char *)alloc_sprintf("NV_SOURCE=%s", nv_source);
    _putenv(strdup(tmpstring_1));
    free(tmpstring_1);

    tmpstring_2 = (char *)alloc_sprintf("NV_COMMON=%s", nv_common);
    _putenv(strdup(tmpstring_2));
    free(tmpstring_2);

    tmpstring_3 = (char *)alloc_sprintf("NV_TOOLS=%s", nv_tools);
    _putenv(strdup(tmpstring_3));
    free(tmpstring_3);

    tmpstring_4 = (char *)alloc_sprintf("NV_GCOMP_ROOT=%s", nv_gcomp_root);
    _putenv(strdup(tmpstring_4));
    free(tmpstring_4);

#ifdef __APPLE__
    tmpstring_5 = (char *)alloc_sprintf("NV_APPLE_TOOLS=%s", nv_apple_tools);
    _putenv(strdup(tmpstring_5));
    free(tmpstring_5);
#endif

    tmpstring_6 = (char *)alloc_sprintf("NVMAKE_VERSION=%s", "2.07");
    _putenv(strdup(tmpstring_6));
    free(tmpstring_6);

    hostos = GETHOSTOS();
    hostarch = GETHOSTARCH();
    if (!GETHOSTARCH || !GETHOSTOS)
    {
        exit(1);
    }

    tmpstring_7 = (char *)alloc_sprintf("NV_HOST_OS=%s", hostos);
    _putenv(strdup(tmpstring_7));
    free(tmpstring_7);

    tmpstring_8 = (char *)alloc_sprintf("NV_HOST_ARCH=%s", hostarch);
    _putenv(strdup(tmpstring_8));
    free(tmpstring_8);

    delstr = nv_source;
    if (debug <= 0)
    {
        compstr = "-s";
        AddEarlyArgument(compstr, nv_source);
    }
    else
    {
        AddArgument("NV_VERBOSE=%d", debug);
    }
    if (debug > 0)
    {
        printf("-----| nvmake version %s |-----\n", "2.07");
        printf("\n");
        printf("nvmake:      NV_SOURCE: [%s]\n", nv_source);
        printf("nvmake:      NV_COMMON: [%s]\n", nv_common);
        printf("nvmake:       NV_TOOLS: [%s]\n", nv_tools);
#ifdef __APPLE__
        printf("nvmake: NV_APPLE_TOOLS: [%s]\n", nv_apple_tools);
#endif
        printf("nvmake:  NV_GCOMP_ROOT: [%s]\n", nv_gcomp_root);
        printf("nvmake:    config file: [%s]\n", config_path);
        printf("\n");
    }
    if ((module_conditions & 16) != 0 && (module_conditions & 8) != 0 )
    {
        module_conditions &= ~16;
    }
    if ((module_conditions & 16))
    {
        AddArgument("clean");
        if (debug > 0)
        {
            puts("nvmake: Cleaning...");
        }
        AllModules = MakeAllModules();
        recursive_itteration = 1;
        if (AllModules && (module_conditions & 1) == 0)
        {
            return AllModules;
        }
        RemoveOneArgument();
    }
    if ((module_conditions & 8))
    {
        AddArgument("clobber");
        if (debug > 0)
        {
            puts("nvmake: Clobbering...");
        }
        AllModules = MakeAllModules();
        recursive_itteration++;
        if (AllModules && (module_conditions & 1) == 0)
        {
            return AllModules;
        }
        RemoveOneArgument();
    }
    if ((module_conditions & 4))
    {
        AddArgument("depend");
        if (debug > 0)
        {
            puts("nvmake: Depending...");
        }
        AllModules = MakeAllModules();
        recursive_itteration++;
        if (AllModules && (module_conditions & 1) == 0)
        {
            return AllModules;
        }
        RemoveOneArgument(0);
    }
    if ((module_conditions & 2))
    {
        AddArgument("build");
        if (debug > 0)
        {
            puts("nvmake: Building...");
        }
        AllModules = MakeAllModules();
        recursive_itteration++;
        RemoveOneArgument();
    }
    if (!recursive_itteration)
        return MakeAllModules();
    return AllModules;
}

void ShowUsage(const char * ver)
{
    //stub;
}

































