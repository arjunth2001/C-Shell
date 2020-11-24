all:
	gcc -g -w  -o shell main.c inputhandlers.c cd.c echo.c ls.c runCommand.c pinfo.c nightswatch.c global.c history.c redirection.c pipe.c setenv.c unsetenv.c jobs.c err.c
