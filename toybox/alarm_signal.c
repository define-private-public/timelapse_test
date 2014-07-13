#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define ALARM_TIME 2

static void interrupt_handler(int sig) {
	printf("%s caught, exiting.\n", strsignal(sig));

	exit(0);
}

static void alarm_handler(int sig) {
	printf("%s caught.\n", strsignal(sig));
	alarm(ALARM_TIME);
}


int main() {
	// Structures needed
	struct sigaction intAction, alarmAction;

	// Setup the signal actions
	sigemptyset(&intAction.sa_mask);
	sigemptyset(&alarmAction.sa_mask);
	intAction.sa_handler = interrupt_handler;
	alarmAction.sa_handler = alarm_handler;

	// Set the signals
	if ((sigaction(SIGINT, &intAction, NULL) == -1) || (sigaction(SIGALRM, &alarmAction, NULL) == -1)) {
		fprintf(stderr, "Couldn't setup signals.\n");
		exit(1);
	}

	// Program "loop"
	printf("Printing a message every two seconds (via alarm()) until SIGINT received.\n");
	alarm(ALARM_TIME);	// Could also use <sys/time.h> timers
	while (1)
		usleep(1);	// Might be kind of bad wer are mixing `usleep()` here.

	return 0;
}

