// Main source file for the timeline_test application

// Includes
#include <iostream>
#include <fstream>
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
#include <sys/statvfs.h>
#include <dirent.h>


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
#define ESTIMATION_MODIFIER 1.15


// Namespaces
using namespace std;


/*== Variables int the global scope ==*/
static unsigned long timeout, duration;
static const char *destDir;
static unsigned int numPictures, numTaken;
static unsigned short durDays, durHours, durMinutes, durSeconds;
static char *savedImagePath= NULL;
static cv::VideoCapture camera;
static cv::Mat image;


/*== Function prototypes ==*/
static void shutdownHandler(int sig);
static void alarmHandler(int sig);
void endTimelapse(int exitReason);


/*== Function definitions ==*/
// SIGINT, SIGTERM, and SIGQUIT are intercepted here
static void shutdownHandler(int sig) {
	// Gracefully shutdown the program
	cout << "Recieved signal(" << sig << "): " << strsignal(sig) << endl;
	cout << "Shuting down application." << endl;

	endTimelapse(EXIT_SUCCESS);
}


// Take a picture and save it on an alarm signal(SIGALRM), then generate a new one
static void alarmHandler(int sig) {
	time_t captureTime;
	bool imageWritten = false;

	// Take the picture
	numTaken++;
	cout << "Taking picture number " << numTaken << ":" << endl;
	captureTime = time(NULL);
	camera.read(image);

	// Chekc for emptyness
	if (!image.empty()) {
		// Save the image
		sprintf(savedImagePath, "%s/%lli.jpg", destDir, (long long)captureTime);
		imageWritten = cv::imwrite(savedImagePath, image);
		image.release();

		// Double check
		if (imageWritten)
			cout << "  Saved to \"" << savedImagePath << "\"" << endl;
		else {
			cerr << "ERROR: Could not save image to \"" << savedImagePath << "\", ending timelapse." << endl;
			endTimelapse(EXIT_FAILURE);
		}
	} else {
		// Image was empty?  shutdown (or possibly continue on, still haven't decided yet
		cerr << "  Image was empty, ending timelapse." << endl;
		endTimelapse(EXIT_FAILURE);
	}

	// Re-setup the alarm to go off
	alarm(timeout);
}


// Releases all OpenCV resources, frees memeory, and write statistics
// For this, function we assume that all global variables have been initialized/allocated; will terminate program as well
void endTimelapse(int exitReason) {
	// Variables for statistics
	ofstream statsFile;
	DIR *dir;
	struct dirent *dirFiles;
	struct stat imgStat;
	unsigned long meanImgSize = 0;
	unsigned long long totalDiskSpace = 0;

	// The cleanup
	camera.release();
	image.release();

	// Just some statistical information
	sprintf(savedImagePath, "%s/stats.txt", destDir);	// improper reuse of varible here, being a bad boy
	statsFile.open(savedImagePath, ofstream::trunc);

	if (statsFile.is_open()) {
		// Loop over all of the iamges to get some info, assume we can poll the directory (no error checking)
		dir = opendir(destDir);
		while ((dirFiles = readdir(dir)) != NULL) {
			sprintf(savedImagePath, "%s/%s", destDir, dirFiles->d_name);
			if (stat(savedImagePath, &imgStat) == 0)
				totalDiskSpace += imgStat.st_size;
		}

		// Write some statistics
		statsFile << "Took " << numTaken << " pictures every " << timeout << " seconds." << endl;
		statsFile << "Duraiton: " << endl;
		statsFile << "  " << durDays << " days" << endl;
		statsFile << "  " << durHours << " hours" << endl;
		statsFile << "  " <<  durMinutes << " minutes" << endl;
		statsFile << "  " << durSeconds << " seconds" << endl;
		statsFile << totalDiskSpace << " bytes of disk space used." << endl;
		if (numTaken > 0)
			statsFile << "Average size of an image is " << (totalDiskSpace / numTaken) << " bytes." << endl;

		statsFile.close();
		
	} else
		cerr << "Could not write down any statistics about the timelapse." << endl;

	// Bit more cleanup
	delete savedImagePath;
	exit(exitReason);
}





// Main program 
int main(int argc, char *argv[]) {
	// Variables needed
	struct stat st;
	struct statvfs vfs;
	struct sigaction shutdownAction, alarmAction;
	unsigned int testImageSize;
	unsigned long long diskSpaceFree, estimatedTotalSize;
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

	// How many seconds long is the who thing?
	duration = durSeconds;
	duration += (durMinutes * 60);
	duration += (durHours * 60 * 60);
	duration += (durDays * 60 * 60 * 24);

	// Also how many pictures will be taken?
	numPictures = duration / timeout;

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

	// Make an estimation on how much disk space this will take up, and check if we have enough
	estimatedTotalSize = (unsigned long long)((float)numPictures * (float)testImageSize * ESTIMATION_MODIFIER);

	n = statvfs(destDir, &vfs);
	if (n == -1) {
		// Hit a snag for some reason
		cerr << "Was not able to get amount of free disk space, reason(" << errno << "): " << strerror(errno) << endl;
		diskSpaceFree = 0;
	} else
		diskSpaceFree = vfs.f_bsize * vfs.f_bfree;	// Success, multipy free blocks times block size

	// Info
	cout << "Taking a photo every " << timeout << " seconds over the course of:" << endl;
	if (durDays > 0)
		cout << durDays << " days, ";
	if (durHours > 0)
		cout << durHours << " hours, ";
	if (durMinutes > 0)
		cout << durMinutes << " minutes, ";
	if (durSeconds > 0)
		cout << durSeconds << " seconds";
	cout << endl;
	cout << numPictures << " will be taken in total, using an estimate of " << estimatedTotalSize << " bytes of disk space." << endl;

	// If they don't have enough space, display a warning
	if (estimatedTotalSize > diskSpaceFree) {
		cout << "WARNING: You might not have enough free disk space for the timelapse." << endl;
		cout << "         You need about " << (estimatedTotalSize - diskSpaceFree) << " more bytes of free space." << endl;
	}


	/*== Add the signal handlers ==*/
	// Setup the signal actions
	sigemptyset(&shutdownAction.sa_mask);
	sigemptyset(&alarmAction.sa_mask);
	shutdownAction.sa_handler = shutdownHandler;
	alarmAction.sa_handler = alarmHandler;

	// Set the signals, sigaction() returns either 0 or -1, we can use it here
	n = sigaction(SIGINT, &shutdownAction, NULL);
	n += sigaction(SIGTERM, &shutdownAction, NULL);
	n += sigaction(SIGQUIT, &shutdownAction, NULL);
	n += sigaction(SIGALRM, &alarmAction, NULL);
	if (n != 0){
		cerr << "Could not setup signals, shutting down." << endl;
		endTimelapse(EXIT_FAILURE);
	}


	/*== Start the capture? ==*/
	cout << endl << "Press Enter to start or ^C to quit." << endl;
	cin.get();


	/*== Main "loop" ==*/
	alarm(timeout);
	if (camera.isOpened()) {
		cout << "Capturing images..." << endl;
		numTaken = 0;

		// Loop until we've taken all of the pictures
		while (numTaken < numPictures) {
//			camera.read(image);		// Leaving these in for reasons
//			image.release();
			usleep(10);				// Take a little nap
		}
	} else {
		cerr << "Was not able to open the camera during the capture phase." << endl;
		exit(1);
	}

	// Finishing things
	endTimelapse(EXIT_SUCCESS);
	return 0;
}

