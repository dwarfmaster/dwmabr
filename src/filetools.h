
#ifndef DEF_FILETOOLS
#define DEF_FILETOOLS

#include <dirent.h>

char** parts(const char* path); // Découpe un chemin : /home/xxx/Documents/img.png -> home, xxx, Documents, img.png
char* filename(const char* path); // /home/xxx/Documents/img.png -> img.png, renvoie NULL si path pointe sur un dossier
char* dir(const char* path); // /home/xxx/Documents/img.png -> /home/xxx/Documents/

int readInt(FILE* f);
unsigned int readUInt(FILE* f);
double readDouble(FILE* f);
char* readWord(FILE* f);

struct dirent** dirContain(DIR* dir, int* length); // Écrit la taille lue dans length, le retour doit être libéré

#endif

