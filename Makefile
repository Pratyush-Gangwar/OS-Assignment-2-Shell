main.out : main.c shell_utils.h stringhandling.c launch.c history.c signal_handler.c launch_pipe.c
	gcc main.c stringhandling.c launch.c history.c signal_handler.c launch_pipe.c -g -o main.out
