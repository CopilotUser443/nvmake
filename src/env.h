#include "util.h"

#ifndef _ENV_H_
#define _ENV_H_

void SetupEnvironment();
char * ExpandVars (const char * var);
char * expandVar (const char * varName, const char * varValue, const char * varArg);

#endif