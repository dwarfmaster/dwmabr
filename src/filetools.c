
#include "filetools.h"

#include <string.h>
#include <stdlib.h>

char** parts(char* path)
{
	size_t nbParts = 0, i = 0;
	char* part = strtok(path, "/");
	char** alls = NULL;

	while(part != NULL)
	{
		++nbParts;
		part = strtok(NULL, "/");
	}

	if(nbParts == 0)
		return  NULL;

	alls = malloc(sizeof(char*) * (nbParts + 1));
	if(alls == NULL)
		return NULL;

	part = strtok(path, "/");
	while(part != NULL)
	{
		alls[i] = part;
		strtok(NULL, "/");
	}

	alls[nbParts-1] = NULL;
	return alls;
}

char* filename(char* path)
{
	char** alls = parts(path);
	if(alls == NULL)
		return NULL;

	size_t i = 0;
	while(alls[i] != NULL)
		++i;
	--i;
	
	char* file = malloc(sizeof(char) * strlen(alls[i]));
	memcpy(file, alls[i], strlen(file));
	free(alls);
	return file;
}

char* dir(char* path)
{
	size_t lenIn = strlen(path);
	char* last = filename(path);
	if(last == NULL)
		return NULL;
	size_t lenFile = strlen(last) - 1; // On ne compte pas le \0 final
	size_t nsize = lenIn - lenFile;
	char* dirp = NULL;

	free(last);
	dirp = malloc(sizeof(char) * nsize);
	if(dirp == NULL)
		return NULL;
	memcpy(dirp, path, nsize - 1);
	dirp[nsize - 1] = '\0';

	return dirp;
}

int readInt(FILE* f)
{
}

unsigned int readUInt(FILE* f)
{
}

double readDouble(FILE* f)
{
}

char* readWord(FILE* f)
{
}

struct dirent** dirContain(DIR* dir, size_t* length)
{
	struct dirent* afile;
	struct dirent** files;
	size_t i;

	*length = 0;
	while( (afile = readdir(dir)) != NULL )
		++(*length);

	files = malloc(sizeof(struct dirent*) * *length);
	if(files == NULL)
		return NULL;

	rewinddir(dir);
	for(i = 0; (afile = readdir(dir)) != NULL; ++i)
		files[i] = afile;

	return files;
}


