
#ifndef DEF_INOTOOLS
#define DEF_INOTOOLS

int ino_init();
int ino_close();
char* ino_error(); // Retourne une str indiquant décrivant l'erreur survenue

typedef int InoWatch;
#define INOERR -1
InoWatch ino_watch(const char* path); // Retourne INOERR en cas de problème, utiliser ino_error pour avoir une description
void ino_unwatch(InoWatch wd);

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
	const char* path;
};

void ino_pollEvent(); // Met à jour les évènements stockés
int ino_getEvent(InoEvent* ev, InoWatch wd); // Retourne 1 tant qu'il reste des évènements

#endif

