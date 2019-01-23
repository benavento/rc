include Make.$(shell uname)

TARG=rc

OFILES=\
	code.$O\
	exec.$O\
	getflags.$O\
	glob.$O\
	here.$O\
	io.$O\
	lex.$O\
	pcmd.$O\
	pfnc.$O\
	simple.$O\
	subr.$O\
	trap.$O\
	tree.$O\
	var.$O\
	y.tab.$O\
	unix.$O\
	havefork.$O\
	prompt-$(PROMPT).$O\

HFILES=\
	rc.h\
	x.tab.h\
	io.h\
	exec.h\
	fns.h\

YFILES=syn.y

all: $(TARG)

$(TARG): $(OFILES)
	$(CC) $(ARCHS) -o $(TARG) $(OFILES) $(LFLAGS)

%.$O: %.c $(HFILES)
	$(CC) $(ARCHS) $(CFLAGS) -c $*.c

y.tab.h y.tab.c: $(YFILES)
	$(YACC) $(YFLAGS) $(YFILES)

x.tab.h: y.tab.h
	cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h

install: all rcmain.unix rc.1
	install -m 755 $(TARG) $(PREFIX)/bin
	install -m 644 rcmain.unix	$(PREFIX)/lib/rcmain
	install -m 644 rc.1 $(PREFIX)/share/man/man1

%: %.c $(HFILES)
	$(CC) $(ARCHS) -o $@ $< $(LFLAGS)

clean:
	rm -f $(TARG) *.$O $(CLEANFILES)
