#include "util.h"
#include "env.h"
#include "cfg.h"
#include "cmd.h"
#include "arg.h"
#include "os.h"
#include "make.h"

int main (
    int argc,
    char **argv,
    char **envp
) {
    bool isCommand;
    //Itterators
    int i;
    int j;
    int k;
    int ModuleDesc;
    int AllModules;
    int debugnum;
    int moduleCount;
    char firstArgChar;
    char * argument;
    char * verbosenum;
    char * dupVar;
    char * NV_SOURCE;
    char * NV_COMMON;
    char * NV_TOOLS;
    char * NV_GCOMP_ROOT;
#ifdef __APPLE__
    char * NV_APPLE_TOOLS;
#endif
    char * NVMAKE_VERSION;
    char * NV_HOST_OS;
    char * NV_HOST_ARCH;
    char * hostOS;
    char * hostArch;
    const char * expandedVar;

    AllModules = 0;
    ModuleDesc = 0;

    memset(&Global, 0, sizeof(Global));
    Global.args = malloc((argc + 2) * sizeof(char *));

    SetupEnvironment();
    ReadBuildConfigFile();
    if ( !Global.GMAKE )
    {
        fprintf(stderr, "nvmake: GMAKE setting missing in build.cfg '%s'\n", Global.config_path);
        exit(1);
    }
    if ( !Global.GMAKE_FORCE_ARGS )
    {
        fprintf(stderr, "nvmake: GMAKE_FORCE_ARGS setting missing in build.cfg '%s'\n", Global.config_path);
        exit(1);
    }
    AddArgument(Global.GMAKE);
    AddArgument(Global.GMAKE_FORCE_ARGS);
    AddArgument("-f makefile.nvmk");
    for ( i = 1; i < argc; i++ )
    {
        argument = strdup(argv[i]);
        if ( !argument )
        {
            fprintf(stderr, "nvmake: failed to allocate memory\n");
            exit(1);
        }
        Global.NV_OPT_STATE = true;
        if ( !strcmp(argument, "-help")
          || !strcmp(argument, "/help")
          || !strcmp(argument, "help")
          || !strcmp(argument, "-h")
          || !strcmp(argument, "/h")
          || !strcmp(argument, "h")
          || !strcmp(argument, "-?")
          || !strcmp(argument, "/?")
          || !strcmp(argument, "?") )
        {
            ShowUsage("2.07");
            exit(0);
        }
        if ( !strncmp(argument, "verbose", 7) )
        {
            verbosenum = strchr(argument, '=');
            if ( verbosenum )
                debugnum = atoi(verbosenum + 1);
            else
                debugnum = 1;
            Global.verboseLevel = debugnum;
        }
        else
        {
            if ( !strcmp(argument, "v") )
            {
                Global.verboseLevel = 1;
                goto LABEL_V;
            }
            if ( strchr(argument, '=') )
            {
LABEL_EQ:   
                AddLiteralArgument(argument);
                goto LABEL_V;
            }
            if ( !strncmp(argument, "-time", 5) )
            {
                Global.GMAKE_TIMER = true;
            }
            else
            {
                if ( *argument == '-' )
                    goto LABEL_EQ;
                if ( *argument == '@' )
                {
                    AddLiteralArgument(argument + 1);
                }
                else
                {
                    firstArgChar = argument[strlen(argument) - 1];
                    if ( firstArgChar == '+' )
                    {
                        Global.NV_OPT_STATE = true;
                        argument[strlen(argument) - 1] = 0;
                    }
                    else if ( firstArgChar == '-' )
                    {
                        Global.NV_OPT_STATE = false;
                        argument[strlen(argument) - 1] = 0;
                    }
                    for ( j = 0; j < Global.commandCount; j++ )
                    {
                        isCommand = false;
                        if ( !strcmp(argument, Global.commands[j]) )
                        {
                            isCommand = true;
                        }
                        else
                        {
                            for ( k = 0; k < Global.aliascount[j]; k++ )
                            {
                                // May need to check if the alias location exists or not
                                if ( !strcmp(argument, Global.aliases[j][k]) )
                                {
                                    isCommand = true;
                                    break;
                                }
                            }
                        }
                        if (isCommand)
                        {
                            if ( !strncmp(Global.commandarg[j], "module ", 7) )
                            {
                                if ( Global.modules_count >= 64 )
                                {
                                    fprintf(stderr, "nvmake: can only handle %d modules, needs recompile. See mlavoie\n", 64);
                                    exit(1);
                                }
                                expandedVar = (const char *)ExpandVars(Global.commandarg[j] + 7);
                                dupVar = strdup(expandedVar);
                                moduleCount = Global.modules_count;
                                Global.modules_count = moduleCount + 1;
                                Global.modules[moduleCount] = dupVar;
                            }
                            else if ( !strcmp(Global.commandarg[j], "nonstop") )
                            {
                                Global.make_type |= 1;
                                AddArgument("-k");
                            }
                            else if ( !strcmp(Global.commandarg[j], "build") )
                            {
                                Global.make_type |= 2;
                            }
                            else if ( !strcmp(Global.commandarg[j], "depend") )
                            {
                                Global.make_type |= 4;
                            }
                            else if ( !strcmp(Global.commandarg[j], "clobber") )
                            {
                                Global.make_type |= 8;
                            }
                            else if ( !strcmp(Global.commandarg[j], "clean") )
                            {
                                Global.make_type |= 16;
                            }
                            else
                            {
                                AddArgument((const char *)ExpandVars(Global.commandarg[j]));
                            }
                            break;
                        }
                    }
                    if ( j == Global.commandCount )
                    {
                        fprintf(stderr, "nvmake: unrecognized argument: %s\n", argument);
                        exit(1);
                    }
                }
            }
        }
LABEL_V:
        free(argument);
    }

    putenv("SHELL=");
    putenv("LIB=");
    putenv("INCLUDE=");
    unsetenv("MAKE");
    unsetenv("MAKEFLAGS");
    unsetenv("MFLAGS");
    if ( !Global.verboseLevel ) putenv("VS_UNICODE_OUTPUT=");
    
    NV_SOURCE = strdup(alloc_sprintf("NV_SOURCE=%s", Global.NV_SOURCE));
    putenv(NV_SOURCE);
    NV_COMMON = strdup(alloc_sprintf("NV_COMMON=%s", Global.NV_COMMON));
    putenv(NV_COMMON);
    NV_TOOLS = strdup(alloc_sprintf("NV_TOOLS=%s", Global.NV_TOOLS));
    putenv(NV_TOOLS);
    NV_GCOMP_ROOT = strdup(alloc_sprintf("NV_GCOMP_ROOT=%s", Global.NV_GCOMP_ROOT));
    putenv(NV_GCOMP_ROOT);
#ifdef __APPLE__
    NV_APPLE_TOOLS = strdup(alloc_sprintf("NV_APPLE_TOOLS=%s", Global.NV_APPLE_TOOLS));
    putenv(NV_APPLE_TOOLS);
#endif
    NVMAKE_VERSION = strdup(alloc_sprintf("NVMAKE_VERSION=%s", "2.07"));
    putenv(NVMAKE_VERSION);

    hostOS = GETHOSTOS();
    hostArch = GETHOSTARCH();
    if ( !hostOS || !hostArch )
        exit(1);
    NV_HOST_OS = strdup(alloc_sprintf("NV_HOST_OS=%s", hostOS));
    putenv(NV_HOST_OS);
    NV_HOST_ARCH = strdup(alloc_sprintf("NV_HOST_ARCH=%s", hostArch));
    putenv(NV_HOST_ARCH);

    if ( Global.verboseLevel > 0 )
    {
        AddArgument("NV_VERBOSE=%d", Global.verboseLevel);
    }
    if ( Global.verboseLevel > 0 )
    {
        printf("-----| nvmake version %s |-----\n", "2.07");
        printf("\n");
        printf("nvmake:      NV_SOURCE: [%s]\n", (const char *)Global.NV_SOURCE);
        printf("nvmake:      NV_COMMON: [%s]\n", (const char *)Global.NV_COMMON);
        printf("nvmake:       NV_TOOLS: [%s]\n", (const char *)Global.NV_TOOLS);
#ifdef __APPLE__
        printf("nvmake: NV_APPLE_TOOLS: [%s]\n", (const char *)Global.NV_APPLE_TOOLS);
#endif
        printf("nvmake:  NV_GCOMP_ROOT: [%s]\n", (const char *)Global.NV_GCOMP_ROOT);
        printf("nvmake:    config file: [%s]\n", (const char *)Global.config_path);
        printf("\n");
    }

    if ( (Global.make_type & 16) != false && (Global.make_type & 8) != false )
        Global.make_type &= -17; //magic const
    if ( (Global.make_type & 16) != false )
    {
        AddArgument("clean");
        if ( Global.verboseLevel > 0 )
        {
            puts("nvmake: Cleaning...");
        }
        AllModules = MakeAllModules();
        ModuleDesc = 1;
        if ( AllModules && (Global.make_type & 1) == false )
            return AllModules;
        RemoveOneArgument();
    }
    if ( (Global.make_type & 8) != false )
    {
        AddArgument("clobber");
        if ( Global.verboseLevel > 0 )
        {
            puts("nvmake: Clobbering...");
        }
        AllModules = MakeAllModules();
        ModuleDesc++;
        if ( AllModules && (Global.make_type & 1) == false )
            return AllModules;
        RemoveOneArgument();
    }
    if ( (Global.make_type & 4) != false )
    {
        AddArgument("depend");
        if ( Global.verboseLevel > 0 )
        {
            puts("nvmake: Depending...");
        }
        AllModules = MakeAllModules();
        ModuleDesc++;
        if ( AllModules && (Global.make_type & 1) == false )
            return AllModules;
        RemoveOneArgument();
    }
    if ( (Global.make_type & 2) != false )
    {
        AddArgument("build");
        if ( Global.verboseLevel > 0 )
        {
            puts("nvmake: Building...");
        }
        AllModules = MakeAllModules();
        ModuleDesc++;
        if ( AllModules && (Global.make_type & 1) == false )
            return AllModules;
        RemoveOneArgument();
    }
    if ( !ModuleDesc )
        return MakeAllModules();
    return AllModules;
}