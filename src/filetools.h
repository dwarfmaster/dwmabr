
#ifndef DEF_FILETOOLS
#define DEF_FILETOOLS

#include <dirent.h>
#include <stdio.h>

// Le retour des fonctions suivantes doit être libéré avec free
char** parts(char* path); // Découpe un chemin : /home/xxx/Documents/img.png -> home, xxx, Documents, img.png
char* filename(char* path); // /home/xxx/Documents/img.png -> img.png
char* dir(char* path); // /home/xxx/Documents/img.png -> /home/xxx/Documents/

int readInt(FILE* f);
unsigned int readUInt(FILE* f);
double readDouble(FILE* f);
char* readWord(FILE* f);

struct dirent** dirContain(DIR* dir, size_t* length); // Écrit la taille lue dans length, le retour doit être libéré

#endif

