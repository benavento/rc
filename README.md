# Rc — The Plan 9 Shell  #

Standalone port of Rc shell written by Tom Duff with edit, history and completion support.

## Building ##

In a terminal run:
```
cd rc
make
make install 

```

## Config ##

### Profile ###

```
/Users/$USER/.rcrc
```

### Completion ###

In _.rcrc__ define a _fn_complete_ funciton.

```
fn err { echo $* >[1=2] }

fn complete_history {
	history | sort -u | fzf -0 --header 'History'
}

fn complete_arg {
}

fn complete_command {
	find $path -maxdepth 1 >[2]/dev/null | sed 's/\/.*\///' | sort -u |
        fzf -q '^'^$1 -0 -1 --header 'Commands' --prompt $prompt(1)
}

fn complete {
#err complete: $#* "$"*"
	if(~ $#* 0)
		exit

	latsp=no
	if(~ $"* *^' ')
		lastsp=yes

	*=`{echo $*}
	switch($#*){
	case 0
		complete_history
	case 1
		if(~ $lastsp yes)
			complete_arg $1
		if not
			complete_command $1
	case *
		exit ''
	}
}
```

## Documentation ##
* [Rc — The Plan 9 Shell](http://9p.io/sys/doc/rc.html)
* [*rc(1)*](http://9p.io/magic/man2html?man=rc&sect=1l)
