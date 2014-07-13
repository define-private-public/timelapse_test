#include <stdio.h>
#include <unistd.h>
#include "cv.h"
#include "highgui.h"

typedef IplImage *(*callback_prototype)(IplImage*);

IplImage *make_it_gray(IplImage* frame) {
	// Allocate space for a new image
	IplImage *gray_frame = 0;
	gray_frame = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
	if (!gray_frame) {
		fprintf(stderr, "!!! cvCrateImage failed!\n");
		return NULL;
	}

	cvCvtColor(frame, gray_frame, CV_RGB2GRAY);
	return gray_frame;
}


void process_video(callback_prototype custom_cb) {
	// Initilize camera
	CvCapture *capture = 0;
	capture = cvCaptureFromCAM(-1);
	if (!capture) {
		fprintf(stderr, "!!! Cannot open initialize webcam!\n");
		return;
	}

	// Create window
	cvNamedWindow("result", CV_WINDOW_AUTOSIZE);
	IplImage *frame = 0;
	char key = 0;

	while (key != 27) {		// ESC
		frame = cvQueryFrame(capture);
		if (!frame) {
			fprintf(stderr, "!!! cvQueryFrame failed!\n");
			break;
		}

		// Execute calllback
		IplImage *processed_frame = (*custom_cb)(frame);

		// Display
		cvShowImage("result", processed_frame);

		// Release resources
		cvReleaseImage(&processed_frame);

		// Exit when user press ESC
		key = cvWaitKey(10);
	}

	// Free Memory
	cvDestroyWindow("result");
	cvReleaseCapture(&capture);
}



int main(int argc, char *argv[]) {
	process_video(make_it_gray);

	return 0;
}

