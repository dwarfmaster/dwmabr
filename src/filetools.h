
#ifndef DEF_FILETOOLS
#define DEF_FILETOOLS

#include <dirent.h>
#include <stdio.h>

#ifndef _DIRENT_HAVE_D_TYPE
#pragma error "struct dirent havent d_type, needed."
#endif

// Le retour des fonctions suivantes doit être libéré avec free, et est égal à NULL en cas d'erreur
char** parts(char* path, size_t* length); // Découpe un chemin : /home/xxx/Documents/img.png -> home, xxx, Documents, img.png, chaque chaine doit être libérée avant de libéré le tout
char* filename(char* path); // /home/xxx/Documents/img.png -> img.png
char* dir(char* path); // /home/xxx/Documents/img.png -> /home/xxx/Documents/
struct dirent** dirContain(DIR* dir, size_t* length); // Écrit la taille lue dans length
void freeAll(char** alls, size_t length); // Facilite la libération du retour de parts
int removeDir(char* path); // Supprime récursivement un dossier

#endif

