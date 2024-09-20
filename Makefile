main.out : main.c shell_utils.h stringhandling.c launch.c history.c signal_handler.c
	gcc main.c stringhandling.c launch.c history.c signal_handler.c -g -o main.out
