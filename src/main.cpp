// Main source file for the timeline_test application

// Includes
#include <iostream>
#include <opencv2/opencv.hpp>		// Using C++ version of OpenCV
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <unistd.h>

// Namespaces
using namespace std;


/*== Function prototypes ==*/
inline void print_usage();		// Prints a usage message


/*== Function definitions ==*/
inline void print_usage() {
	cout << "./timelapse_test <timeout> <duration> <destination>" << endl;
}


int main(int argc, char *argv[]) {
	cout << "Capture a timelapse!" << endl;

	// TODO arugment processing
	// TODO directory checking

	// First check the arugments
	if (argc < 4) {
		cout << "Not enough arguements." << endl;
		print_usage();
		exit(0);
	}

	return 0;
}

