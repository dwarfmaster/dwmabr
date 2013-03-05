
#ifndef DEF_FILETOOLS
#define DEF_FILETOOLS

#include <dirent.h>
#include <stdio.h>

// Le retour des fonctions suivantes doit être libéré avec free, et est égal à NULL en cas d'erreur
char** parts(char* path); // Découpe un chemin : /home/xxx/Documents/img.png -> home, xxx, Documents, img.png
char* filename(char* path); // /home/xxx/Documents/img.png -> img.png
char* dir(char* path); // /home/xxx/Documents/img.png -> /home/xxx/Documents/
struct dirent** dirContain(DIR* dir, size_t* length); // Écrit la taille lue dans length

#endif

