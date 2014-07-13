//#include "cv.h"
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;


int main(int argc, char *argv[]) {
	CvCapture *capture = NULL;
	Mat frame, frameCopy, image;

	capture = cvCaptureFromCAM(0);	// Default == 0
	if (!capture)
		cout << "No camera detected" << endl;
	
	cvNamedWindow("result", 1);

	if (capture) {
		cout << "In capture ..." << endl;
		while (true) {
			IplImage *iplImg = cvQueryFrame(capture);
			frame = iplImg;

			if (frame.empty())
				break;

//			if (iplImg->origin == IPL_ORIGIN_TL)
//				frame.copyTo(frameCopy);
//			else
//				flip(frame, frameCopy, 0);

			cvShowImage("result", iplImg);

			if (waitKey(10) >= 0)
				cvReleaseCapture(&capture);
		}

		waitKey(0);
	}

	cvDestroyWindow("result");

	return 0;
}

