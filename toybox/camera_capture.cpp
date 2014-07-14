//#include "cv.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <unistd.h>

using namespace std;


int main(int argc, char *argv[]) {
	cv::VideoCapture capture;
	cv::Mat image;
	int samples, maxSamples = 50;

	// Need a filename
	if (argc < 2)
		return 0;

	capture.open(0);	// Default camera


	if (capture.isOpened()) {
		cout << "In capture ..." << endl;
	
		// let the camera warm up with a few samples
		while (samples < maxSamples) {
			capture.read(image);
			image.release();
			samples++;
		}

		capture.read(image);
		if (!image.empty()) {
			cv::imwrite(argv[1], image);
			cout << "Saving image..." << endl;
		}
	}

	// cleanup
	capture.release();
	image.release();


	return 0;
}

