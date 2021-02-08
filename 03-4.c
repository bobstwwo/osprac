#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
	pid_t pid = fork();

	if (pid == -1) {
	/* ошибка */
	printf("Error on program start\n");
	exit(-1);
	} else if (pid == 0) {
	/* ребенок */
	printf("\nThis is a child process\n");
	printf("Launching the another programm process\n\n");
	execl("/bin/cat", "/bin/cat", "sample.c", 0);
	} else {
	/* родитель */
	printf("This is a parent process\n");
  }
}
