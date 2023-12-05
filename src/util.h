#include <stdbool.h>

char * alloc_sprintf(char * fmt, ...);
void NormalizeSlashes(char * path, char replacement);
void configError(char * fmt, ...);
void MyChdir(const char * path, ...);
unsigned int spawn(
    char * binpath,
    const char ** arguments
);
int getToken(
    char * tok,
    char ** array
);
