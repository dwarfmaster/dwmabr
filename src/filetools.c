
#include "filetools.h"

#include <string.h>
#include <stdlib.h>

char* duplicate(const char* src)
{
	size_t size = sizeof(char) * strlen(src);
	char* new = malloc(size);
	if(new == NULL)
		return NULL;

	return memcpy(new, src, size);
}

char** parts(char* path, size_t* length)
{
	size_t nbParts = 0, i = 0;
	char* part = NULL;
	char** alls = NULL;

	char* usedPath = duplicate(path);
	if( usedPath == NULL )
		return NULL;
	
	printf("Path = %s\n", usedPath);
	part = strtok(usedPath, "/");
	while(part != NULL)
	{
		++nbParts;
		printf("Part : %s\n", part);
		part = strtok(NULL, "/");
	}

	if(nbParts == 0)
		return  NULL;

	alls = malloc(sizeof(char*) * nbParts);
	if(alls == NULL)
		return NULL;

	strcpy(usedPath, path);
	printf("Path = %s\n", usedPath);
	part = strtok(usedPath, "/");
	while(part != NULL)
	{
		alls[i] = duplicate(part);
		if(alls[i] == NULL)
		{
			size_t j;
			for(j = 0; j < i; ++j)
				free(alls[i]);
			return NULL;
		}
		++i;

		printf("Part : %s\n", alls[i]);
		part = strtok(NULL, "/");
	}
	free(usedPath);

	if(length != NULL)
		*length = nbParts;
	printf("End\n");
	return alls;
}

char* filename(char* path)
{
	printf("Begin filename.\n");
	char** alls = parts(path);
	if(alls == NULL)
		return NULL;

	size_t i = 0;
	while(alls[i] != NULL)
	{
		printf("\tAll[%i] = %s\n", i, alls[i]);
		++i;
	}
	--i;
	printf("Size : %i", i);
	
	char* file = malloc(sizeof(char) * strlen(alls[i]));
	memcpy(file, alls[i], strlen(file));
	free(alls);

	printf("Fin de filename.");
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


