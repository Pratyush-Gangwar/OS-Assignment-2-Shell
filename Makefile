main.out : main.c shell_utils.h stringhandling.c launch.c history.c
	gcc main.c stringhandling.c launch.c history.c -g -o main.out
