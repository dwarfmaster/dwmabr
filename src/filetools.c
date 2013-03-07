
#include "filetools.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char* duplicate(const char* src)
{
	size_t size = sizeof(char) * strlen(src);
	char* new = malloc(size);
	if(new == NULL)
		return NULL;

	strcpy(new, src);
	return new;
}

char** parts(const char* path, size_t* length)
{
	size_t nbParts = 0, i = 0;
	char* part = NULL;
	char** alls = NULL;

	char* usedPath = duplicate(path);
	if( usedPath == NULL )
		return NULL;
	
	part = strtok(usedPath, "/");
	while(part != NULL)
	{
		++nbParts;
		part = strtok(NULL, "/");
	}

	if(nbParts == 0)
		return  NULL;

	alls = malloc(sizeof(char*) * nbParts);
	if(alls == NULL)
		return NULL;

	strcpy(usedPath, path);
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
		part = strtok(NULL, "/");
	}
	free(usedPath);

	if(length != NULL)
		*length = nbParts;
	return alls;
}

char* filename(const char* path)
{
	size_t length = 0;
	char** alls = parts(path, &length);
	if(alls == NULL)
		return NULL;
	size_t pos = length - 1;

	char* file = malloc(sizeof(char) * strlen(alls[pos]));
	if( file == NULL )
		return NULL;
	memcpy(file, alls[pos], strlen(alls[pos]));
	freeAll(alls, length);

	return file;
}

char* dir(const char* path)
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

void freeAll(char** alls, size_t length)
{
	size_t i;
	for(i = 0; i < length; ++i)
		free(alls[i]);
	free(alls);
}

int removeDir(const char* path)
{
	size_t length, i;
	struct dirent** contents;
	DIR* dir = opendir(path);
	if(dir == NULL)
		return -1;

	contents = dirContain(dir, &length);
	if(contents == NULL)
		return -1;

	// Le deux pour sauter le . et le ..
	for(i = 2; i < length; ++i)
	{
		char* pathfile = malloc(sizeof(char) * (strlen(contents[i]->d_name) + strlen(path) + 1)); // Le +1 pour rajouter le '/'
		if(pathfile == NULL)
		{
			free(contents);
			closedir(dir);
			return -1;
		}
		strcpy(pathfile, path);
		strcat(pathfile, "/");
		strcat(pathfile, contents[i]->d_name);

		if(contents[i]->d_type == DT_DIR)
		{
			if(removeDir(pathfile) < 0)
			{
				free(contents);
				closedir(dir);
				return -1;
			}
		}
		else
			unlink(pathfile);

		free(pathfile);
	}

	free(contents);
	closedir(dir);
	rmdir(path);
	return 0;
}


