#include "util.h"

#ifndef _MAKE_H_
#define _MAKE_H_

int MakeAllModules();
int ExecuteGnuMake();
void MyChdir(const char * dir, ...);

#endif