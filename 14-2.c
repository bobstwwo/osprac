#include <stdio.h>
#include <signal.h>

void my_handler(int nsig) {
	if (nsig == SIGINT) {
		printf("Signal: %d. Ctrl+C is ignored.\n", nsig);
	}
	else if (nsig == SIGQUIT) {
		printf("Signal: %d. Ctrl+4 is ignored.\n", nsig);
	}
}

int main() {
	// Instead of ignoring the signals, pass the handler funtion.
	signal(SIGINT, my_handler);
	signal(SIGQUIT, my_handler);
	// Inifinitely print the messages, until the process is killed externally.
	while (1);
	return 0;
}
