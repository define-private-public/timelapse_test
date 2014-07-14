// Main source file for the timeline_test application

// Includes
#include <iostream>
#include <opencv2/opencv.hpp>		// Using C++ version of OpenCV
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <csignal>
#include <cerrno>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


// Definitions
#define ARG_TIMEOUT 1
#define ARG_DURATION 2
#define ARG_DESTINATION 3
#define MAX_DAYS 99
#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59
#define RWX_PERMS 0777


// Namespaces
using namespace std;


/*== Function prototypes ==*/



/*== Function definitions ==*/



int main(int argc, char *argv[]) {
	// Variables needed
	unsigned long timeout, duration;
	unsigned short durDays, durHours, durMinutes, durSeconds;
	const char *destDir;
	int n;

	// Welcome message
	cout << "Capture a timelapse!" << endl;

	/*== Check the arguements ==*/
	// Make sure we have enough arguments
	if (argc < 4) {
		cout << "Not enough arguements, useage:" << endl;
		cout << "  ./timelapse_test <timeout> <duration> <destination>" << endl;
		exit(0);
	}

	// Make sure timeout is greater than zero
	if (atoi(argv[ARG_TIMEOUT]) <= 0) {
		cerr << "<timeout> value is negative, need a positive integer." << endl;
		exit(1);
	} else
		timeout = atoi(argv[ARG_TIMEOUT]);

	// Read the duration string
	n = sscanf(argv[ARG_DURATION], "%hu:%hu:%hu:%hu", &durDays, &durHours, &durMinutes, &durSeconds);
	if (n < 4) {
		// Make sure we read the correct amount, print an error otherwise
		cerr << "\"" << argv[ARG_DURATION] << "\" is an invalid duration string, please use the format of:" << endl;
		cerr << "  \"DD:mm:hh:ss\"" << endl;
		exit(1);
	} else if ((durDays > MAX_DAYS) || (durHours > MAX_HOURS) || (durMinutes > MAX_MINUTES) || (durSeconds > MAX_SECONDS)) {
		cerr << "\"" << argv[ARG_DURATION] << "\" is an invalid duration string, please make sure to follow the limits of:" << endl;
		cerr << "  [0-" << MAX_DAYS << "] Days" << endl;
		cerr << "  [0-" << MAX_HOURS << "] Hours" << endl;
		cerr << "  [0-" << MAX_MINUTES << "] Minutes" << endl;
		cerr << "  [0-" << MAX_SECONDS << "] Seconds" << endl;
		exit(1);
	} else if ((durDays + durHours + durMinutes + durSeconds) == 0) {
		cerr << "Timelapse duration of 0 total seconds.  Are you serious?" << endl;
		exit(1);
	}

	// Check the destination directory
	destDir = argv[ARG_DESTINATION];

	if (access(destDir, F_OK) == -1) {
		// Destination dir doesn't exist, make it
		cout << "\"" << destDir << "\" doesn't exist, making it." << endl;

		// Make the directory
		if (mkdir(destDir, RWX_PERMS) == -1) {
			// Could not make the directoy, print an error message and exit
			cerr << "Could not make the directory \"" << destDir << "\":" << endl;

			// Pick a reason
			switch (errno) {
				case EACCES:
					cerr << "  Could not write to the parent directory." << endl;
					break;
				case ENOENT:
					cerr << "  Pathname doesn't exist." << endl;
					break;
				case EPERM:
					cerr << "  Directory creation not permitted. Are you the parent's owner?" << endl;
					break;
				case EROFS:
					cerr << "  Parent is a read-only filesystem." << endl;
					break;
				default:
					cerr << "  Other error(" << errno << "): " << strerror(errno) << endl;
			}

			exit(1);
		}

	} else if (access(destDir, R_OK | W_OK | X_OK) == -1) {
		// Cannot use the destination directory; exit
		cerr << "Cannot use directory \"" << destDir << "\"." << endl;
		cerr << "Please make sure that you can read, write, and execute it." << endl;
		exit(1);
	}

	
	// TODO: take a picture and make sure there is enough space
	// TODO: add all signal handlers


	
	// Stats
	cout << "Timeout (seconds): " << timeout << endl;
	cout << "Days: " << durDays << endl;
	cout << "Hours: " << durHours << endl;
	cout << "Minutes: " << durMinutes << endl;
	cout << "Seconds: " << durSeconds << endl;
	cout << "Destination Directory: " << destDir << endl;


	return 0;
}

