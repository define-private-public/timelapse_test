// Main source file for the timeline_test application

// Includes
#include <iostream>
#include <opencv2/opencv.hpp>		// Using C++ version of OpenCV
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <csignal>
#include <unistd.h>

// Definitions
#define ARG_TIMEOUT 1
#define ARG_DURATION 2
#define ARG_DESTINATION 3
#define MAX_DAYS 99
#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59

// Namespaces
using namespace std;


/*== Function prototypes ==*/
inline void print_usage();		// Prints a usage message



/*== Function definitions ==*/
inline void print_usage() {
	cout << "./timelapse_test <timeout> <duration> <destination>" << endl;
}


int main(int argc, char *argv[]) {
	// Variables needed
	unsigned long timeout, duration;
	unsigned short durDays, durHours, durMinutes, durSeconds;
	const char *destination;
	int n;

	// Welcome message
	cout << "Capture a timelapse!" << endl;

	/*== Check the arguements ==*/
	// Make sure we have enough arguments
	if (argc < 4) {
		cout << "Not enough arguements." << endl;
		print_usage();
		exit(0);
	}

	// Make sure timeout is greater than zero
	if (atoi(argv[ARG_TIMEOUT]) <= 0) {
		cerr << "<timeout> value is negative, need a positive integer." << endl;
		exit(1);
	} else
		timeout = atoi(argv[ARG_TIMEOUT]);

	// Read the duration string
	n = sscanf(argv[ARG_DURATION], "%u:%u:%u:%u", &durDays, &durHours, &durMinutes, &durSeconds);
	if (n < 4) {
		// Make sure we read the correct amount, print an error otherwise
		cerr << "\"" << argv[ARG_DURATION] << "\" is an invalid duration string, please use the format of:" << endl;
		cerr << "\"DD:mm:hh:ss\"" << endl;
		exit(1);
	} else if ((durDays > MAX_DAYS) || (durHours > MAX_HOURS) || (durMinutes > MAX_MINUTES) || (durSeconds > MAX_SECONDS)) {
		cerr << "\"" << argv[ARG_DURATION] << "\" is an invalid duration string, please make sure to follow the limits of:" << endl;
		cerr << "[0-" << MAX_DAYS << "] Days" << endl;
		cerr << "[0-" << MAX_HOURS << "] Hours" << endl;
		cerr << "[0-" << MAX_MINUTES << "] Minutes" << endl;
		cerr << "[0-" << MAX_SECONDS << "] Seconds" << endl;
		exit(1);
	}

	// TODO check directory

	cout << "Timeout (seconds): " << timeout << endl;
	cout << "Days: " << durDays << endl;
	cout << "Hours: " << durHours << endl;
	cout << "Minutes: " << durMinutes << endl;
	cout << "Seconds: " << durSeconds << endl;


	return 0;
}

