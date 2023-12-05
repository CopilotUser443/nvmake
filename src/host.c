#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(__linux__) || defined(__APPLE__)
    #include <sys/utsname.h>
    struct utsname host;
#endif

#include "host.h"

char * GETHOSTOS()
{
#ifdef _WIN32
    return "Windows";
#endif
#if defined(__linux__) || defined(__APPLE__)
    long long result;

    if (uname(&host)) 
    {
        result = 0LL;
        fprintf(stderr, "Unable to determine host operating system!\n");
    }
    else if (!strcmp(host.sysname, "Linux"))
    {
        return "Linux";
    }
    else if (!strcmp(host.sysname, "Solaris"))
    {
        return "SunOS";
    }
    else if (!strcmp(host.sysname, "FreeBSD"))
    {
        return "FreeBSD";
    }
    else if (!strcmp(host.sysname, "Darwin"))
    {
        return "macosx";
    }
    else
    {
        result = 0LL;
        fprintf(stderr, "Unrecognized host operating system '%s'!\n", host.sysname);
    }
    return (char *)result;
#endif 
}

char * GETHOSTARCH() 
{
#ifdef _WIN32
    return "amd64";
#endif
#if defined(__linux__) || defined(__APPLE__)
    long long result;

    if (uname(&host))
    {
        result = 0LL;
        fprintf(stderr, "Unable to determine host architecture!\n");
    } 
    else if ( !strcmp(host.machine, "x86")
           || !strcmp(host.machine, "i386")
           || !strcmp(host.machine, "i486")
           || !strcmp(host.machine, "i586")
           || !strcmp(host.machine, "i686")
           || !strcmp(host.machine, "i86pc") )
    {
        return "x86";
    }
    else if (!strcmp(host.machine, "x86_64") || !strcmp(host.machine, "amd64")) {
        return "amd64";
    }
    else 
    {
        result = 0LL;
        fprintf(stderr, "Unrecognized host architecture '%s'!\n", host.machine);
    }
    return (char *)result;
#endif
}