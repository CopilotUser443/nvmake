#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#ifdef __unix__
#include <sys/utsname.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <spawn.h>
#elif defined(_WIN32)
#include <windows.h>
#endif
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>

#ifndef _UTIL_H_
#define _UTIL_H_

#ifdef _WIN32
#define putenv _putenv
#endif

#define engwiki "https://engwiki/index.php/Nvmake"
#define BYTEn(x, n)   (*((unsigned char*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)

struct _Global {
    bool NV_OPT_STATE;
    bool GMAKE_TIMER;
    int verboseLevel;
    int currentLine;
    int commandCount;
    int aliascount[255];
    int argCount;
    int modules_count;
    int make_type;
    char config_path[1055];
    char NV_SOURCE[1024];
    char NV_COMMON[1024];
    char NV_TOOLS[1024];
    char NV_GCOMP_ROOT[1024];
    char ExpandVars_szWork[4096];
#ifdef __APPLE__
    char NV_APPLE_TOOLS[1024];
#endif
    char * SET;
    char * GMAKE;
    char * GMAKE_FORCE_ARGS;
    char * commands[255];
    char * commandarg[255];
    char * commanddescription[255];
    char * modules[255];
    char ** args;
    char * aliases[255][5];
};

char * NormalizeSlashes(char * path, char slash);
void configError(char * fmt, ...);
bool getToken (char * token, char ** context);
bool _iscsym (char character);
char * alloc_sprintf (char * str, ...);
unsigned int spawn(char * binpath, char ** arguments);
#ifdef _WIN32
int unsetenv(const char *name);
#endif


extern struct _Global Global;

#endif