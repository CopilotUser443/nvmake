#include "cfg.h"
#include "env.h"

int ReadBuildConfigFile()
{
    bool spaceCondition;
    bool aliasCondition;
    signed int i;
    char * Itteration; 
    char * expandedVar;
    char * expandedGMAKEVar;
    char * aliasStr;
    char __little[1024];
    char content[1032];
    FILE * cfgFile;

    cfgFile = fopen((const char *)Global.config_path, "r");
    if ( !cfgFile )
    {
        fprintf(stderr, "nvmake: failed to open build config file: %s\n", (const char *)Global.config_path);
        exit(1);
    }
    while ( fgets(content, 1024, cfgFile) )
    {
        Global.currentLine++;
        for ( Itteration = content; ; Itteration++ )
        {
            spaceCondition = false;
            if ( *Itteration )
                spaceCondition = isspace(*Itteration) != false;
            if ( !spaceCondition )
                break;
        }
        if ( *Itteration
          && *Itteration != '\n'
          && *Itteration != '#' )
        {
            if ( !getToken(__little, &Itteration) )
                configError("expected %s", "command");
            if ( !strstr("WINDOWS-UNIX-MACOSX", __little) )
                goto LABEL_PARSE;
#ifdef __linux__
            if ( !strcmp(__little, "UNIX") )
#elif defined(__APPLE__)
            if ( !strcmp(__little, "MACOSX") )
#elif defined(_WIN32)
            if ( !strcmp(__little, "WINDOWS") )
#endif
            {
                if ( !getToken(__little, &Itteration) )
                    configError("expected %s", "command");
LABEL_PARSE:
                if ( !strcmp(__little, "SET") )
                {
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s, variable name");
                    if ( !strcmp(__little, "NV_BRANCH") )
                    {
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "=");
                        if ( strcmp(__little, "=") )
                            configError("expected %s", "=");
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "string constant");
                        Global.SET = strdup(__little);
                    }
                    else if ( !strcmp(__little, "GMAKE") )
                    {
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "=");
                        if ( strcmp(__little, "=") )
                            configError("expected %s", "=");
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "string constant");
                        Global.GMAKE = strdup(__little);
                        expandedVar = ExpandVars(Global.GMAKE);
                        free(Global.GMAKE);
                        Global.GMAKE = strdup(expandedVar);
                    }
                    else if ( !strcmp(__little, "GMAKE_FORCE_ARGS") )
                    {
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "=");
                        if ( strcmp(__little, "=") )
                            configError("expected %s", "=");
                        if ( !getToken(__little, &Itteration) )
                            configError("expected %s", "string constant");
                        Global.GMAKE_FORCE_ARGS = strdup(__little);
                        expandedGMAKEVar = ExpandVars(Global.GMAKE_FORCE_ARGS);
                        free(Global.GMAKE_FORCE_ARGS);
                        Global.GMAKE_FORCE_ARGS = strdup(expandedGMAKEVar);
                    }
                    else {
                        configError("unexpected variable in SET: %s", __little);
                    }
                }
                else if ( !strcmp(__little, "MAKECMD") )
                {
                    if ( Global.commandCount > 255 )
                        configError("too many configurable commands (exceeded maximum of %d)", 256);
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s", "string constant");
                    Global.commands[Global.commandCount] = strdup(__little);
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s", "string constant");
                    Global.commandarg[Global.commandCount] = strdup(__little);
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s", "string constant");
                    Global.commanddescription[Global.commandCount] = strdup(__little);
                    Global.commandCount++;
                }
                else if ( !strcmp(__little, "CMDALIAS") )
                {
                    aliasCondition = false;
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s", "string constant");
                    aliasStr = strdup(__little);
                    for ( i = 0; i < Global.commandCount; i++ )
                    {
                        if ( !strcmp(Global.commands[i], aliasStr) )
                        {
                            aliasCondition = true;
                            break;
                        }
                    }
                    if ( !aliasCondition )
                        configError("command %s is not defined so it cannot be aliased", aliasStr);
                    Global.aliascount[i]++;
                    if ( Global.aliascount[i] >= 5 )
                        configError("too many aliases for %s", aliasStr);
                    if ( !getToken(__little, &Itteration) )
                        configError("expected %s", "string constant");
                    Global.aliases[i][Global.aliascount[i] - 1] = strdup(__little);
                }
                else if ( strcmp(__little, "DEPEND") )
                {
                    configError("expected SET, MAKECMD, CMDALIAS or DEPEND: got [%s]", __little);
                }
            }
        }  
    }
    return fclose(cfgFile);
}