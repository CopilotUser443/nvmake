#include "env.h"

void SetupEnvironment()
{
    bool definedSource;
    bool lastCharSet;
    struct stat cfgStat;
    struct stat toolStat;
    struct stat gcompStat;
    char * charDiv;
    char * lsChar;
    char * toolsPath;
    char * gcompPath;
    char * gcompSource;
    char * NV_SOURCE;
    char * NV_TOOLS;
    char * NV_GCOMP_ROOT;
    char currentDir[1024];
    char buildCfgPath[1055];
    const char *cfgPath;
#ifdef __APPLE__
    struct stat appleToolsStat;
    char * appleToolsPath;
    char * NV_APPLE_TOOLS;
#endif

    NV_SOURCE = getenv("NV_SOURCE");
    NV_TOOLS = getenv("NV_TOOLS");
    NV_GCOMP_ROOT = getenv("NV_GCOMP_ROOT");
#ifdef __APPLE__
    NV_APPLE_TOOLS = getenv("NV_APPLE_TOOLS");
#endif

    if ( NV_SOURCE )
    {
        strcpy(Global.NV_SOURCE, NV_SOURCE);
    }
    else
    {
        definedSource = false;
        getcwd(currentDir, sizeof(currentDir));
#ifdef __unix__
        NormalizeSlashes(currentDir, '/');
#elif defined(_WIN32)
        NormalizeSlashes(currentDir, '\\');
#endif
        lsChar = &currentDir[strlen(currentDir) - 1];
        while ( true )
        {
            sprintf(buildCfgPath, "%s/drivers/common/build/build.cfg", currentDir);
            if ( stat(buildCfgPath, &cfgStat) != -1 )
                break;
            while ( true )
            {
                lastCharSet = false;
                if ( lsChar > currentDir )
#ifdef __unix__
                    lastCharSet = *lsChar != '/';
#elif defined(_WIN32)
                    lastCharSet = *lsChar != '\\';
#endif
                if ( !lastCharSet )
                    break;
                --lsChar;
            }
#ifdef __unix__
            if ( *lsChar == '/' )
#elif defined(_WIN32)
            if ( *lsChar == '\\' )
#endif
            {
                charDiv = lsChar--;
                *charDiv = 0;
            }
            if ( lsChar <= currentDir )
                goto LABEL_UNDEFINED_FINDCOMMON;
        }
        definedSource = true;
LABEL_UNDEFINED_FINDCOMMON:
        if ( !definedSource )
        {
            getcwd(currentDir, sizeof(currentDir));
#ifdef __unix__
            NormalizeSlashes(currentDir, '/');
#elif defined(_WIN32)
            NormalizeSlashes(currentDir, '\\');
#endif
            lsChar = &currentDir[strlen(currentDir) - 1];
            while ( true )
            {
                sprintf(buildCfgPath, "%s/common/build/build.cfg", currentDir);
                if ( stat(buildCfgPath, &cfgStat) != -1 )
                    break;
                while ( true )
                {
                    lastCharSet = false;
                    if ( lsChar > currentDir )
#ifdef __unix__
                        lastCharSet = *lsChar != '/';
#elif defined(_WIN32)
                        lastCharSet = *lsChar != '\\';
#endif
                    if ( !lastCharSet )
                        break;
                    --lsChar;
                }
#ifdef __unix__
                if ( *lsChar == '/' )
#elif defined(_WIN32)
                if ( *lsChar == '\\' )
#endif
                {
                    charDiv = lsChar--;
                    *charDiv = 0;
                }
                if ( lsChar <= currentDir )
                {
                    if ( !definedSource )
                    {
                        fprintf(stderr, "nvmake: Unable to locate build.cfg.  \n");
                        fprintf(stderr, "        Therefore, unable to automatically determine NV_SOURCE.\n");
                        fprintf(stderr, "        See %s for more information.\n", engwiki);
                        exit(1);
                    }
                }
                definedSource = true;
            }
        }
        strcpy(Global.NV_SOURCE, currentDir);
    }
#ifdef __unix__
    NormalizeSlashes(Global.NV_SOURCE, '/');
#elif defined(_WIN32)
    NormalizeSlashes(Global.NV_SOURCE, '\\');
#endif
    cfgPath = '\0';

    memset(buildCfgPath, 0, sizeof(buildCfgPath));
    memset(currentDir, 0, sizeof(currentDir));

    sprintf(buildCfgPath, "%s/drivers/common/build/build.cfg", (const char *)Global.NV_SOURCE);
    if ( stat(buildCfgPath, &cfgStat) != -1)
        cfgPath = "/drivers/common";
    sprintf(buildCfgPath, "%s/common/build/build.cfg", (const char *)Global.NV_SOURCE);
    if ( stat(buildCfgPath, &cfgStat) != -1)
        cfgPath = "/common";
    if ( !cfgPath )
    {
        fprintf(stderr, "nvmake: NV_SOURCE: %s\n", (const char *)Global.NV_SOURCE);
        fprintf(stderr, "        Unable to derive NV_COMMON from NV_SOURCE.\n");
        fprintf(stderr, "        Does %s/(drivers/)common/build/build.cfg exist?\n", Global.NV_SOURCE);
        fprintf(stderr, "        See %s for more information.\n", "https://engwiki/index.php/Nvmake");
        exit(1);
    }
    strcpy(Global.NV_COMMON, Global.NV_SOURCE);
    strcat(Global.NV_COMMON, cfgPath);
#ifdef __unix__
    NormalizeSlashes(Global.NV_COMMON, '/');
#elif defined(_WIN32)
    NormalizeSlashes(Global.NV_COMMON, '\\');
#endif
    definedSource = false;
    getcwd(currentDir, sizeof(currentDir));
#ifdef __unix__
    NormalizeSlashes(currentDir, '/');
#elif defined(_WIN32)
    NormalizeSlashes(currentDir, '\\');
#endif
    lsChar = &currentDir[strlen(currentDir) - 1];
    while ( true )
    {
        sprintf(Global.config_path, "%s/build.cfg", currentDir);
        if ( stat((const char *)Global.config_path, &cfgStat ) != -1 )
            break;
        while ( true )
        {
            lastCharSet = false;
            if ( lsChar > currentDir )
#ifdef __unix__
                lastCharSet = *lsChar != '/';
#elif defined(_WIN32)
                lastCharSet = *lsChar != '\\';
#endif
            if ( !lastCharSet )
                break;
            --lsChar;
        }
#ifdef __unix__
        if ( *lsChar == '/' )
#elif defined(_WIN32)
        if ( *lsChar == '\\' )
#endif
        {
            charDiv = lsChar--;
            *charDiv = 0;
        }
        if ( lsChar <= currentDir )
            goto LABEL_FINDCOMMON;
    }
    definedSource = true;

LABEL_FINDCOMMON:
    if ( !definedSource )
    {
        sprintf(Global.config_path, "%s/common/build/build.cfg", (const char *)Global.NV_SOURCE);
        
        if ( stat((const char *)Global.config_path, &cfgStat ) == -1 )
        {
            sprintf(Global.config_path, "%s/drivers/common/build/build.cfg", (const char *)Global.NV_SOURCE);
            if ( stat((const char *)Global.config_path, &cfgStat ) != -1 )
                definedSource = true;
        }
        else
            definedSource = true;
        
        if ( !definedSource )
        {
            fprintf(stderr, "nvmake: Unable to locate build.cfg file.\n");
            fprintf(stderr, "        See %s for more information.\n", engwiki);
            exit(1);
        }
    }
    if ( NV_TOOLS )
    {
        strcpy(Global.NV_TOOLS, NV_TOOLS);
#ifdef __unix__
        NormalizeSlashes(Global.NV_TOOLS, '/');
#elif defined(_WIN32)
        NormalizeSlashes(Global.NV_TOOLS, '\\');
#endif
    }
    else
    {
        definedSource = false;
        strcpy(Global.NV_TOOLS, Global.NV_SOURCE);
#ifdef __unix__
        NormalizeSlashes(Global.NV_TOOLS, '/');
#elif defined(_WIN32)
        NormalizeSlashes(Global.NV_TOOLS, '\\');
#endif
        while ( !definedSource )
        {
#ifdef __unix__
            toolsPath = strrchr((const char *)Global.NV_TOOLS, '/');
#elif defined(_WIN32)
            toolsPath = strrchr((const char *)Global.NV_TOOLS, '\\');
#endif
            if ( !toolsPath )
                break;
            strcpy(toolsPath + 1, "tools");
            toolsPath[6] = '\0';
            if ( stat((const char *)Global.NV_TOOLS, &toolStat) == -1 )
                *toolsPath = 0;
            else
                definedSource = true;
        }
        if ( !definedSource )
        {
            fprintf(stderr, "nvmake: NV_SOURCE: %s\n", (const char *)Global.NV_SOURCE);
            fprintf(stderr, "        Unable to derive NV_TOOLS from NV_SOURCE.\n");
            fprintf(stderr, "        Please set NV_TOOLS.\n");
            fprintf(stderr, "        See %s for more information.\n", engwiki);
            exit(1);
        }
    }
#ifdef __APPLE__
    if ( NV_APPLE_TOOLS )
    {
        strcpy(Global.NV_APPLE_TOOLS, NV_APPLE_TOOLS);
        NormalizeSlashes(Global.NV_APPLE_TOOLS, '/');
    }
    else
    {
        definedSource = false;
        strcpy(Global.NV_APPLE_TOOLS, NV_SOURCE);
        NormalizeSlashes(NV_APPLE_TOOLS, '/');
        while ( !definedSource )
        {
            appleToolsPath = strrchr((const char *)Global.NV_APPLE_TOOLS, '/');
            if ( !appleToolsPath )
                break;
            strcpy(appleToolsPath + 1, "apple/tools");
            appleToolsPath[12] = '\0';
            if ( stat((const char *)Global.NV_APPLE_TOOLS, &appleToolsStat) == -1 )
                *appleToolsPath = 0;
            else
                definedSource = true;
        }
        if ( !definedSource )
            strcpy(Global.NV_APPLE_TOOLS, "<NV_APPLE_TOOLS_NOT_FOUND>");
    }
#endif
    if ( NV_GCOMP_ROOT )
    {
        strcpy(Global.NV_GCOMP_ROOT, NV_GCOMP_ROOT);
#ifdef __unix__
        NormalizeSlashes(Global.NV_GCOMP_ROOT, '/');
#elif defined(_WIN32)
        NormalizeSlashes(Global.NV_GCOMP_ROOT, '\\');
#endif
    }
    else
    {
        definedSource = false;
        strcpy(Global.NV_GCOMP_ROOT, NV_TOOLS);
#ifdef __unix__
        NormalizeSlashes(Global.NV_GCOMP_ROOT, '/');
#elif defined(_WIN32)
        NormalizeSlashes(Global.NV_GCOMP_ROOT, '\\');
#endif
        gcompPath = strstr((const char *)Global.NV_GCOMP_ROOT, "tools");
        if ( gcompPath )
        {
            strcpy(gcompPath, "gcomp");
            gcompPath[5] = '\0';
            if ( stat((const char *)Global.NV_GCOMP_ROOT, &gcompStat) != -1 )
                definedSource = true;
        }
        if ( !definedSource )
        {
            getcwd(Global.NV_GCOMP_ROOT, sizeof(Global.NV_GCOMP_ROOT));
#ifdef __unix__
            NormalizeSlashes(Global.NV_GCOMP_ROOT, '/');
#elif defined(_WIN32)
            NormalizeSlashes(Global.NV_GCOMP_ROOT, '\\');
#endif 
            while ( !definedSource )
            {
#ifdef __unix__
                gcompSource = strrchr((const char *)Global.NV_GCOMP_ROOT, '/');
#elif defined(_WIN32)
                gcompSource = strrchr((const char *)Global.NV_GCOMP_ROOT, '\\');
#endif
                if ( !gcompSource )
                    break;
                strcpy(gcompSource + 1, "gcomp");
                gcompSource[5] = '\0';
                if ( stat((const char *)Global.NV_GCOMP_ROOT, &gcompStat ) == -1)
                    *gcompSource = 0;
                else
                    definedSource = true;
            }
        }
        if ( !definedSource )
        {
            strcpy(Global.NV_GCOMP_ROOT, "<gCompDirNotFound>");
            if ( Global.verboseLevel > 0)
            {
                fprintf(stderr, "nvmake: NV_SOURCE: %s\n", (const char *)Global.NV_SOURCE);
                fprintf(stderr, "        NV_COMMON: %s\n", (const char *)Global.NV_COMMON);
                fprintf(stderr, "         NV_TOOLS: %s\n", (const char *)Global.NV_TOOLS);
                fprintf(stderr, "        Unable to derive NV_GCOMP_ROOT from NV_TOOLS or NV_SOURCE.\n");
                fprintf(stderr, "         NV_GCOMP_ROOT set to: %s\n", (const char *)Global.NV_GCOMP_ROOT);
            }
        }
    }
    return;
}

char * ExpandVars (
    const char * var
) {
    const char *varBool;

    strcpy(Global.ExpandVars_szWork, var);
    expandVar(Global.ExpandVars_szWork, "<source>", Global.NV_SOURCE);
    expandVar(Global.ExpandVars_szWork, "<tools>", Global.NV_TOOLS);
    expandVar(Global.ExpandVars_szWork, "<gcomp>", Global.NV_GCOMP_ROOT);
    varBool = "0";
    if ( Global.NV_OPT_STATE )
        varBool = "1";
    expandVar(Global.ExpandVars_szWork, "<optState>", varBool);
    return Global.ExpandVars_szWork;
}

char * expandVar (
    const char * varName,
    const char * varValue,
    const char * varArg
) {
    char * result;
    char * fmtres;
    size_t varValueSize;
    size_t varArgSize;
    size_t resultSize;

    varValueSize = strlen(varValue);
    varArgSize = strlen(varArg);
    result = strstr(varName, varValue);
    fmtres = result;
    if ( result )
    {
        resultSize = strlen(result);
        memmove(&fmtres[varArgSize], &fmtres[varValueSize], resultSize - varValueSize);
        return memcpy(fmtres, varArg, varArgSize);
    }
    return result;
}