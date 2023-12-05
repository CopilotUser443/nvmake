#include <stdio.h>
#include <string.h>

extern char ExpandVars_szWork[4096];
extern char * nv_source;
extern char * nv_tools;
extern char * nv_gcomp_root;

extern int nv_opt_state;

char * expandVar(
    const char * var,
    const char * expand,
    const char * exponent
) {
    char * result;
    char * memstr;
    size_t var_size;
    size_t expand_size;
    size_t exponent_size;

    exponent_size = strlen(exponent);
    expand_size = strlen(expand);
    result = strstr(var, expand);
    memstr = result;
    if (result)
    {
        var_size = strlen(result);
        memmove(&memstr[exponent_size], &memstr[expand_size], var_size - expand_size + 1);
        return (char *)memcpy(memstr, exponent, exponent_size);
    }
    return result;
}


void * ExpandVars(char * var)
{
    const char * tmpvar;

    strcpy(ExpandVars_szWork, var);
    expandVar(ExpandVars_szWork, "<source>", nv_source);
    expandVar(ExpandVars_szWork, "<tools>", nv_tools);
    expandVar(ExpandVars_szWork, "<gcomp>", nv_gcomp_root);
    tmpvar = "0";
    if ( nv_opt_state ) tmpvar = "1";
    expandVar(ExpandVars_szWork, "<optState>", tmpvar);
    return ExpandVars_szWork;
}