#include <signal.h>

int main() {
	// Ignore both SIGINT and SIGQUIT.
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	// Inifinite loop, wait until termination.
	while (1);
	return 0;
}
