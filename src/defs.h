#include <stdlib.h>
#include <stdio.h>

#define nvmake_ver "decomp-0.0.1"

#include "types.h"

extern char **environ;

int argi; //Argument Count, DAT_10000b380
char ** args; //Arguments, DAT_10000ab80
char * target; //Argument Target, DAT_10000ab88
NVCCHAR line; //Error line, DAT_10001039c
int launchm; //Launch Mode, DAT_1000103ac
int launchparam; // Launch Parameter, DAT_10000a970
char * makepath; //"make" Binary path, DAT_10000b388, GMAKE
char * ExpandVars_szWork; //"make" Arguments, _ExpandVars.szWork, GMAKE_FORCE_ARGS
char * Global; //NV Source path, _Global, NV_SOURCE
char * nv_tools; //NV Tools path, DAT_100009d70, NV_TOOLS
char * nv_gcomp_root; //NV Compilation Root, DAT_100009970, NV_GCOMP_ROOT
int currdat; //Current Rooted Data, DAT_1000103a8
int gmake_condition; //GNU Make Condition, DAT_10000ab78
char ** modules; //NV Modules, DAT_10000a978
int module_condition; //NV Module Conditions, DAT_10000a974
char * config_file; //%s/build.cfg, DAT_10000a570
char * SET; //Environment Variable "SET", DAT_1000103a0, This value is literally referenced ONCE! What's the point? 
char * make_forced_args; //GMAKE_FORCE_ARGS, DAT_10000b390
int ccommand_count; //Configurable Commands count, DAT_100010398
char ** ccommands; //Configurable commands, DAT_10000b398, Value presumed
char ** cmodules; //Configurable modules, DAT_10000b3d8, Value presumed
char ** cnodes; //Configurable nodes, DAT_10000b3e0, Value presumed
char ** aliases; //Aliases, DAT_10000b3d0
char * nv_common; //NV Common Tools path, DAT_100009570, NV_COMMON
char * nv_apple_tools; //NV Apple Tools path, DAT_10000a170, NV_APPLE_TOOLS