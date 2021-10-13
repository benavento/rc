#include "rc.h"
#include "io.h"

int mypid;

/* globals associated with lex.c and subr.c */
int doprompt = 1;	/* is it time for a prompt? */
int lastdol;	/* was the last token read '$' or '$#' or '"'? */
int lastword;	/* was the last token read a word or compound word terminator? */
char tok[NTOK];
int ndot = 0; // FIXME - never decreases?
int lastc;
int lastword;
int kidpid;

io *rc_err;

/* globals associated with exec.c */
char *promptstr;
tree *cmdtree;
thread *runq;
code *codebuf;				/* compiler output */
int ntrap;				/* number of outstanding traps */
int trap[NSIG];				/* number of outstanding traps per type */
int eflagok;			/* kludge flag so that -e doesn't exit in startup */

char *argv0="rc";
var *gvar[NVAR];                                /* hash for globals */

int nerror = 0;		/* number of errors encountered during compilation */
