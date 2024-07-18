#include "util.h"

#ifndef _ARG_H_
#define _ARG_H_

char * AddArgument (const char * args, ...);
char * AddLiteralArgument (char * arg);
void AddEarlyArgument (const char * arg, ...);
void RemoveOneArgument();
char * DUP_ARGUMENT (const char * arg);
#endif