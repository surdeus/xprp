#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

static char *getprop(Atom atom);
static void setprop(Atom atom, char *value);

static Display *dpy;
static Window win;

static char *argv0;

int
main(int argc, char *argv[])
{
	char *value = NULL;
	Atom atom;
	argv0 = argv[0] ;

	if(!(dpy = XOpenDisplay(NULL))) {
		fprintf(stderr, "%s: Cannot open display.\n", argv0);
		return 1 ;
	}
	switch(argc) {
	case 4:
		value = argv[3];
	case 3:
		atom = XInternAtom(dpy, argv[2], True);
		win = strtoul(argv[1], 0, 0);
		break;
	default:
		fprintf(stderr, "Usage: %s <xid> <atom> [<value>] [-v]\n", argv0);
		return 1;
	}
	if(value)
		setprop(atom, value);
	else {
		if(!(value = getprop(atom))) {
			fprintf(stderr, "%s: Cannot get atom.\n", argv0);
			return 1 ;
		}
		printf("%s\n", value);
		XFree(value);
	}
	XCloseDisplay(dpy);
	return 0;
}

char *
getprop(Atom atom)
{
	int di;
	unsigned long dl;
	unsigned char *p = NULL;
	Atom da;

	XGetWindowProperty(dpy, win, atom, 0, BUFSIZ, False, XA_STRING, &da, &di, &dl, &dl, &p);
	return (char *)p;
}

void
setprop(Atom atom, char *value)
{
	XChangeProperty(dpy, win, atom, XA_STRING, 8, PropModeReplace,
			(unsigned char *)value, strlen(value));
}
