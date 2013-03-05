
#include "inotools.h"

#include <unistd.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static int inoInstance;
static char* errormsg;

struct stackEv
{
	InoEvent event;
	struct stackEv* next;
};

struct watch
{
	int wd;
	const char* path;
	struct stackEv* first;
};
static struct watch* watches;
static unsigned int sizeW; // Taille réelle du tableau
static unsigned int usedW; // Taille utilisée du tableau

int ino_init()
{
	inoInstance = inotify_init();
	if( inoInstance < 0 )
	{
		switch( errno )
		{
			case EMFILE:
				errormsg = "The user limit on the total number of inotify instances has been reached.";
				break;
			case ENFILE:
				errormsg = "The user limit on the total number of file descriptors has been reached.";
				break;
			case ENOMEM:
				errormsg = "Insufficient kernel memory is available.";
				break;
			default:
				errormsg = "Unknown error.";
				break;
		}
		return -1;
	}

	errormsg = "";
	watches = NULL;
	sizeW = 0;
	usedW = 0;

	return 0;
}

int ino_close()
{
	close(inoInstance);
}

char* ino_error()
{
	return errormsg;
}

InoWatch ino_watch(const char* path)
{
	struct watch watch;
	watch.first = NULL;
	watch.path = path;
	watch.wd = inotify_add_watch(inoInstance, path, IN_ATTRIB | IN_CLOSE_WRITE | IN_CREATE | IN_DELETE | IN_MOVED_TO | IN_MOVED_FROM | IN_MOVE_SELF | IN_DELETE_SELF);

	if( watch.wd < 0 )
	{
		switch( errno )
		{
			case EACCES:
				errormsg = "Read access to the given file is not permitted.";
				break;
			case EBADF:
				errormsg = "The given file descriptor is not valid.";
				break;
			case EFAULT:
				errormsg = "The given file is outside the process's accessible address space.";
				break;
			case EINVAL:
				errormsg = "The given event mask contains no valid events.";
				break;
			case ENOENT:
				errormsg = "A directory in the path does not exist or is a dangling symbolic link.";
				break;
			case ENOMEM:
				errormsg = "Insufficient kernel memory was available.";
				break;
			case ENOSPC:
				errormsg = "The user limit on the total number of inotify watches was reached or the kernel failed to allocate a needed resource.";
				break;
			default:
				errormsg = "Unknown error.";
				break;
		}
		return -1;
	}
	
	++usedW;
	if( usedW > sizeW )
	{
		unsigned int lastSize = sizeW;
		sizeW += 10;
		struct watch *tmp = malloc(sizeof(struct watch) * sizeW);
		if( tmp == NULL )
		{
			errormsg = "Can't get memory for the watch stack.";
			sizeW = lastSize;
			--usedW;
			return -1;
		}

		memcpy(tmp, watches, sizeof(struct watch) * lastSize);
		tmp[usedW - 1] = watch;

		if( watches != NULL )
			free(watches);
		watches = tmp;
	}

	return watch.wd;
}

void ino_unwatch(InoWatch wd)
{
	unsigned int i;
	for(i = 0; i < usedW; ++i)
	{
		if( watches[i].wd == wd )
		{
			inotify_rm_watch(inoInstance, watches[i].wd);

			while( ino_getEvent(NULL, wd) ) {}; // On vide la file d'events

			memmove(&watches[i], &watches[i+1], usedW - (i+1));
			--usedW;
			
			if( sizeW - usedW > 10 )
			{
				sizeW -= 10;
				struct watch *tmp = malloc(sizeof(struct watch) * sizeW);
				if( tmp == NULL )
				{
					errormsg = "Can't get memory for the watch stack.";
					sizeW += 10;
					return;
				}
				memcpy(tmp, watches, sizeof(struct watch) * sizeW);

				if( watches != NULL )
					free(watches);
				watches = tmp;

				return;
			}
		}
	}
}

void ino_pollEvent()
{
	const unsigned int event_size = sizeof(struct inotify_event);
	const unsigned int buffer_size = 1024 * (event_size + 16);
	char buffer[buffer_size];

	int length = read(inoInstance, buffer, buffer_size);
	ssize_t i;
	unsigned int j;

	for(i = 0; i < length;)
	{
		struct inotify_event* event = (struct inotify_event*) &buffer[i];
		for(j = 0; j < usedW; ++j)
		{
			if( watches[j].wd == event->wd )
			{
				InoEvent ev;
				if( event->len )
					ev.path = event->name;
				else
					ev.path = watches[j].path;

				if( event->mask & IN_ISDIR )
					ev.dir = 1;
				else
					ev.dir = 0;

				if( event->mask & IN_ATTRIB )
					ev.action = ATTRIB;
				else if( event->mask & IN_CLOSE_WRITE )
					ev.action = WRITE;
				else if( event->mask & IN_CREATE
						|| event->mask & IN_MOVED_TO )
					ev.action = CREATE;
				else if( event->mask & IN_DELETE
						|| event->mask & IN_MOVED_FROM )
					ev.action = DELETE;
				else if( event->mask & IN_MOVE_SELF
						|| event->mask & IN_DELETE_SELF )
					ev.action = END;

				struct stackEv* sev = malloc(sizeof(struct stackEv));
				sev->event = ev;
				sev->next = NULL;

				if( watches[j].first == NULL )
					watches[j].first = sev;
				else
				{
					struct stackEv* aev = watches[j].first;
					while(aev->next != NULL) {}
					aev->next = sev;
				}

				j = usedW;
			}
		}

		i += event_size + event->len;
	}
}

int ino_getEvent(InoEvent* ev, InoWatch wd)
{
	unsigned int i;

	for(i = 0; i < usedW; ++i)
	{
		if( watches[i].wd == wd )
		{
			struct stackEv* sev = watches[i].first;
			if( sev == NULL )
				return 0;
			else
			{
				if( ev != NULL )
					*ev = sev->event;
				watches[i].first = sev->next;
				free(sev);
			}
			return 1;
		}
	}
	return 0;
}


