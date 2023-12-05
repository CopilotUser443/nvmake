//TODO: Create a wiki page.
const char * nvmake_wiki = "https://engwiki/index.php/Nvmake";

int argi; //Addr: &Global + 2180
int nv_opt_state; //Addr: &Global + 7310
int line; //Addr: &Global + 7307
int debug;  //Addr: &Global + 1536
int launchm; //Addr: &Global + 7311
int make_conditions; //Addr: &Global + 1666
int module_conditions; //Addr: &Global + 1537
int ccommands_count; //Addr: &Global + 7306
char ExpandVars_szWork[4096]; //Addr: &ExpandVars_szWork
const char ** args; //Addr: &Global + 834
const char ** envs;
const char ** modules; //Addr: &Global + 769
char ccommands[1024][20]; //Addr: &Global + 2186
char cmodules[1024][20]; //Addr: &Global + 2202
char cnodes[1024][20]; //Addr: &Global + 2204
char aliases[1024][20]; //Addr: &Global + 2200
char unknown[1024][20];
char * nv_source; //Addr: &Global
char * nv_tools; //Addr: &Global + 768
char * nv_gcomp_root; //Addr: &Global + 512
char * nv_common; //Addr: &Global + 1024
char * config_path; //Addr: &Global + 640
char * nv_apple_tools; //Addr: &Global + 256
char * SET; //ambiguous, Addr: &Global + 7308
char * makepath; //Addr: &Global + 2182
char * make_force_args; //Addr: &Global + 2184









