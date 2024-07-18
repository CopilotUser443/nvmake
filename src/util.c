#include "util.h"

struct _Global Global;
extern char **environ;

char * NormalizeSlashes (
    char * path, 
    char slash
) {
    char * normalizedPath;

    while ( true )
    {
        normalizedPath = path;
        if ( !*path )
            break;
        if ( *path == '\\' || *path == '/')
            *path = slash;
        ++path;
    }
    return normalizedPath;
}

void configError (
    char * fmt,
    ...
) {
    char err[4096];
    va_list va;
    char * fmterr;

    fmterr = fmt;
    va_start(va, fmt);
    vsprintf(err, fmt, va);
    fprintf(stderr, "nvmake: error in build.cfg: %s at line %d\n", err, Global.currentLine);
    exit(1);
}

bool getToken (
    char * token,
    char ** context
) {
    char * tmpChar;
    char * mvChar;
    char * tmpChar2;
    char * mvChar2;
    char * tmpChar3;
    char * mvChar3;
    bool spaceCondition;
    bool csymCondition;
    bool existCondition;
    bool definitiveCondition;
    bool tokenInv;
    char * tmpToken;

    tmpToken = token;
    while ( true )
    {
        tokenInv = false;
        if ( **context )
        {
            spaceCondition = true;
            if ( !isspace(**context) )
                spaceCondition = **context == '\n';
            tokenInv = spaceCondition;
        }
        if ( !tokenInv )
            break;
        ++*context;
    }
    if ( **context == '"' )
    {
        ++*context;
        while ( true )
        {
            existCondition = false;
            if ( **context )
                existCondition = **context != '"';
            if ( !existCondition )
                break;
            tmpChar = (*context)++;
            mvChar = tmpToken++;
            *mvChar = *tmpChar;
        }
        if ( **context != '"' )
        {
            fprintf(stderr, "nvmake: expected ending quote in config file\n");
            exit(1);
        }
        ++*context;
    }
    else if ( _iscsym((char)**context) )
    {
        while ( true )
        {
            csymCondition = false;
            if ( **context )
                csymCondition = _iscsym((char)**context) != false;
            if ( !csymCondition )
                break;
            tmpChar2 = (*context)++;
            mvChar2 = tmpToken++;
            *mvChar2 = *tmpChar2;
        }
    }
    else
    {
        while ( true )
        {
            definitiveCondition = false;
            if ( **context )
            {
                if ( !_iscsym((char)**context) )
                    definitiveCondition = **context != '"';
            }
            if ( !definitiveCondition )
                break;
            tmpChar3 = (*context)++;
            mvChar3 = tmpToken++;
            *mvChar3 = *tmpChar3;
        }
    }
    *tmpToken = 0;
    return token != tmpToken;
}

bool _iscsym (
    char character 
) {
    bool csymCondition;
    
    csymCondition = true;
    if ( !isalnum(character) )
        return character == '_';
    return csymCondition;
}

char * alloc_sprintf (
    char * str,
    ...
) {
    char * allocstr;
    char * tmpstr;
    char tmpchar;
    size_t strsize;
    size_t vsnsize;
    va_list va;

    allocstr = str;
    strsize = 0;
    va_start(va, str);
    vsnsize = (size_t)vsnprintf(&tmpchar, 1, str, va);
    strsize += vsnsize;
    tmpstr = (char *)malloc(strsize++);
    if ( !tmpstr )
    {
        fprintf(stderr, "nvmake: failed to allocate %ld bytes of memory\n", strsize);
        exit(1);
    }
    va_end(va);
    va_start(va, str);
    vsprintf(tmpstr, allocstr, va);
    return tmpstr;
}

unsigned int spawn(
    char * binpath,
    char ** arguments
) {
#ifdef __unix__
    pid_t forked_process;
    pid_t wait_process;
    int wait_token;
    unsigned int status = 0;

    if (binpath)
    {
        forked_process = fork();
        if (forked_process == -1)
        {
            status = -1;
            fprintf(stderr, "Failed to fork `%s`: %s\n", binpath, strerror(errno));
        }
        else 
        {
            if (!forked_process)
            {
                execve(binpath, arguments, environ);
                fprintf(stderr, "Failed to execve `%s`: %s\n", binpath, strerror(errno));
                exit(1);
            }
            wait_process = waitpid(forked_process, &wait_token, 0);
            if ((wait_token & 0x7F) != 0)
            {
                fprintf(stderr, "`%s` did not terminate normally: %s\n", binpath, strerror(errno));
            }
            else if (BYTE1(wait_token))
            {
                fprintf(stderr, "`%s` exited with status %d\n", binpath, BYTE1(wait_token));
                return -1U;
            }
            else if (wait_process == -1)
            {
                fprintf(stderr, "waitpid() encountered an error: %s\n", strerror(errno));
                return -1U;
            }
            else
            {
                return 0;
            }
        }    
    }
    else
    {
        return -1U;
    }
    return status;
#endif
#ifdef _WIN32
    //We can't fork a process under windows
    int64_t status;
    status = _spawnve(P_WAIT, binpath, arguments, _environ);
    if (errno) {
        fprintf(stderr, "`%s` did not terminate normally: %s\n", binpath, strerror(errno));
        return -1U;
    }
    if (status != -1 && status != 0) {
        fprintf(stderr, "`%s` exited with status %d\n", binpath, status);
        return -1U;
    }
    if (status == -1 ) {
        fprintf(stderr, "spawnve() encountered an error: %s\n", strerror(errno));
        return -1U;
    }
    return 0;
#endif
}