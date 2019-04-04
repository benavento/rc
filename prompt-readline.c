#include "rc.h"
#include "fns.h"
#include "io.h"
#include "exec.h"
#include "getflags.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

#define DEBUG(f, a...) \
	if(0){}else{pfmt(err, "\n%s: "f,  __FUNCTION__, ## a);flush(err);}

#define COMPLETE_FN 		"complete"
#define COMPLETE_RESULTS 	"rc_complete_results"

static char*
matches_rl(const char *s, int i)
{
    word *w;

	for(w=vlook(COMPLETE_RESULTS)->val; w; w=w->next, i--)
		if(i == 0)
			return strdup(w->word);
	return NULL;
}

static char**
complete_rl(const char *text, int p0, int p1)
{
	code complete[32];
	thread *oldrunq;
	char **l;
	int i, dot;
	struct stat st;

	/* prevent completion fallback */
	//rl_attempted_completion_over = 1;

	if(vlook(COMPLETE_FN)->fn == NULL)
		return NULL;

	rl_line_buffer[p1] = '\0';
	oldrunq = runq;
	memset(complete, 0, sizeof complete);
	i = 0;
	complete[i++].i = 1;
	complete[i++].f = Xmark;
	complete[i++].f = Xbackq;
	dot = i;
	complete[i++].i = 0;
	complete[i++].f = Xmark;
	complete[i++].f = Xword;
	complete[i++].s = (char*)rl_line_buffer;
	complete[i++].f = Xword;
	complete[i++].s = COMPLETE_FN;
	complete[i++].f = Xsimple;
	complete[i++].f = Xexit;
	complete[dot].i = i;
	complete[i++].f = Xmark;
	complete[i++].f = Xword;
	complete[i++].s = COMPLETE_RESULTS;
	complete[i++].f = Xassign;
	complete[i++].f = Xreturn;
	start(complete, 1, NULL);

	while(runq != oldrunq){
		if(flag['r']) pfnc(err, runq);
		(*runq->code[runq->pc++].f)();
		if(ntrap) dotrap();
	}

	l = rl_completion_matches(text, matches_rl);

	/* if dir append '/' if not already there */
	if(l==NULL && stat(text, &st)==0 && S_ISDIR(st.st_mode) && text[(p1-p0)-1]!='/'){
		 l = emalloc(sizeof(char*)*2);
		 l[0] = emalloc(strlen(text)+2);
		 strcpy(l[0], text);
		 strcat(l[0], "/");
	}
	return l;
}

static void
savehist(void)
{
	write_history(NULL);
}

static void
read_rl(void)
{
	static int first = 1;
	io *f = runq->cmdfd;
	char *s;
	long n;

	if(first){
		/* I/O */
		rl_readline_name = "rc";	/* for .editrc */
		rl_outstream = fdopen(err->fd, "w");	// stderr

		/* signals */
		// rl_catch_signals = 0;

		/* complete */
		rl_completion_append_character = '\0';
		rl_completer_quote_characters = "'";
		rl_attempted_completion_function = complete_rl;

		/* history */
		if (read_history(NULL)!=0 && errno!=ENOENT)
			pfmt(err, "rc: read_history: %s\n", strerror(errno));
		atexit(savehist);
		first = 0;
	}

	s = readline(promptstr);
	if(s == NULL)
		return;

	n = strlen(s);
	assert(n < NBUF-1);
	strcpy(f->buf, s);
	f->buf[n++] = '\n';
	f->bufp = f->buf;
	f->ebuf = f->buf+n;

	add_history(s);
	free(s);
}

void
pprompt(void)
{
	var *prompt;

	if(runq->iflag){
		flush(err);
		read_rl();
		prompt = vlook("prompt");
		if(prompt->val && prompt->val->next)
			promptstr = prompt->val->next->word;
		else
			promptstr="\t";
	}
	runq->lineno++;
	doprompt = 0;
}

void
exechistory(void)
{
	HIST_ENTRY *e;
	struct io out[1];
	int i;

	setstatus("");
	out->fd = mapfd(1);
	out->bufp = out->buf;
	out->ebuf = &out->buf[NBUF];
	out->strp = 0;
	for(i=history_base; i<history_length; i++){
		e = history_get(i);
		if(e == NULL)
		break;
		pfmt(out, "%s\n", e->line);
	}
	poplist();
}
