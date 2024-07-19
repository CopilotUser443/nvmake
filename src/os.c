#include "os.h"

char * GETHOSTOS()
{
    char * OS;
#ifdef __unix__
    struct utsname uts;

    if ( uname(&uts) )
    {
        OS = "\0";
        fprintf(stderr, "Unable to determine host operating system!\n");
    }
    else if ( !strcmp(uts.sysname, "Linux") )
    {
        return "Linux";
    }
    else if ( !strcmp(uts.sysname, "Solaris") )
    {
        return "SunOS";
    }
    else if ( !strcmp(uts.sysname, "FreeBSD") )
    {
        return "FreeBSD";
    }
    else if ( !strcmp(uts.sysname, "Darwin") )
    {
        return "macosx";
    }
    else
    {
        OS = "\0";
        fprintf(stderr, "Unrecognized host operating system '%s'!\n", uts.sysname);
    }
#elif defined(_WIN32)
    OS = "windows";
#elif 
#warning "Unrecognized build environment!"
    OS = "\0";
#endif
    return OS;
}

char * GETHOSTARCH()
{
    char * arch;
#ifdef __unix__
    struct utsname uts;

    if ( uname(&uts) )
    {
        arch = "\0";
        fprintf(stderr, "Unable to determine host architecture!\n");
    }
    else if ( !strcmp(uts.machine, "x86")
           || !strcmp(uts.machine, "i386")
           || !strcmp(uts.machine, "i486")
           || !strcmp(uts.machine, "i586")
           || !strcmp(uts.machine, "i686")
           || !strcmp(uts.machine, "i86pc") )
    {
        arch = "x86";
    }
    else if ( !strcmp(uts.machine, "x86_64") || !strcmp(uts.machine, "amd64") )
    {
        arch = "amd64";
    }
    else
    {
        arch = "\0";
        fprintf(stderr, "Unrecognized host architecture '%s'!\n", uts.machine);
    }
#elif defined(_WIN32)
    arch = "amd64";
#elif
#warning "Unrecognized build environment!"
    arch = "\0";
#endif
    return arch;
}