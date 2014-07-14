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
#include <sys/types.h>
#include <sys/stat.h>


// Definitions
#define ARG_TIMEOUT 1
#define ARG_DURATION 2
#define ARG_DESTINATION 3
#define MAX_DAYS 99
#define MAX_HOURS 23
#define MAX_MINUTES 59
#define MAX_SECONDS 59
#define RWX_PERMS 0777
#define DEFAULT_CAMERA 0
#define TEST_IMAGE_FILENAME "test.jpg"


// Namespaces
using namespace std;


/*== Function prototypes ==*/



/*== Function definitions ==*/



int main(int argc, char *argv[]) {
	// Variables needed
	struct stat st;
	unsigned long timeout, duration;
	unsigned short durDays, durHours, durMinutes, durSeconds;
	const char *destDir;
	unsigned int testImageSize;
	char *savedImagePath= NULL;
	int n;
	cv::VideoCapture camera;
	cv::Mat image;

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
	n = atoi(argv[ARG_TIMEOUT]);
	if (n <= 0) {
		cerr << "<timeout> value is negative, need a positive integer." << endl;
		exit(1);
	} else
		timeout = n;

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

	// Allocate enough space for the image filepath buffer
	savedImagePath = new char[strlen(destDir) + 32];		// Should be more than enough



	/*== Last minute checks ==*/
	// Open up the camera
	camera.open(DEFAULT_CAMERA);

	// Check that the opening was successful
	if (camera.isOpened()) {
		// Make a capture so we can estimate the amount of filespace needed.
		camera.read(image);
		if (!image.empty()) {
			// First save the image to the disk
			sprintf(savedImagePath, "%s/%s", destDir, TEST_IMAGE_FILENAME);
			cv::imwrite(savedImagePath, image);

			// Use the stat fuction to get the size
			stat(savedImagePath, &st);
			testImageSize = st.st_size;

			// Delete the test image
			n = unlink(savedImagePath);
			if (n == -1) {
				// don't exit, just print a warning
				cout << "Warning, wasn't able to delete test image.  Reason(" << errno << "): " << strerror(errno) << endl;
			}
		} else {
			cerr << "Was able to open the camera but not able to capture an image." << endl;
			exit(1);
		}
		
		image.release();
	} else {
		cerr << "Was not able to open the camera." << endl;
		exit(1);
	}

	
	// TODO take a picture and estimate if there is enough space
	// 		Let the user decice to continue or not
	// TODO add all signal handlers
	// TODO Ask for confirmation before starting
	//      remove directory he says no.

	
	// Stats
	cout << "Timeout (seconds): " << timeout << endl;
	cout << "Days: " << durDays << endl;
	cout << "Hours: " << durHours << endl;
	cout << "Minutes: " << durMinutes << endl;
	cout << "Seconds: " << durSeconds << endl;
	cout << "Destination Directory: " << destDir << endl;
	cout << "Test image size (bytes): " << testImageSize << endl;

	
	delete savedImagePath;
	return 0;
}

