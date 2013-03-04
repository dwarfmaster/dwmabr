
#ifndef DEF_INOTOOLS
#define DEF_INOTOOLS

int ino_init();
int ino_close();

enum InoAction{ATTRIB, // Modifications permissions, timestamp ...
	WRITE, // Fichier modifié (fermeture en écriture)
	CREATE, // Fichier créé
	DELETE, // Fichier supprimé
	END // Fichier/dossier surveillé supprimé/déplacé ...
};

typedef struct InoEvent InoEvent;
struct InoEvent
{
	enum InoAction action;
	int dir;
	char* path;
};

#endif

