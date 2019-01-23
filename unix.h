

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <inttypes.h>
#include <string.h>

#ifndef NSIG
#define NSIG 32
#endif

#ifndef ERRMAX
#define ERRMAX 128
#endif

/* plan 9 compatibility */
#define RFPROC 1
#define RFFDG 1
#define RFNOTEG 1

#define uintptr uintptr_t
#define Rune	wchar_t

#define nil ((void*)0)

/* in case uchar, etc. are built-in types */
#define uchar	_fmtuchar
#define ushort	_fmtushort
#define uint	_fmtuint
#define ulong	_fmtulong
#define vlong	_fmtvlong
#define uvlong	_fmtuvlong

typedef unsigned char		uchar;
typedef unsigned short		ushort;
typedef unsigned int		uint;
typedef unsigned long		ulong;
typedef unsigned long long	uvlong;

#define OREAD	O_RDONLY
#define OWRITE	O_WRONLY
#define ORDWR	O_RDWR
#define OCEXEC	0

extern char *argv0;
extern int chartorune(wchar_t *, const char *);
extern int rfork(int);
extern void errstr(char *, int);
#define rerrstr errstr

extern void exechistory(void);

