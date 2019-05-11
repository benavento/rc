#include "rc.h"
#include "fns.h"
#include "io.h"
#include "exec.h"

void
pprompt(void)
{
	var *prompt;
	if(runq->iflag){
		pstr(err, promptstr);
		flush(err);
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
	setstatus("not implemented");
}
