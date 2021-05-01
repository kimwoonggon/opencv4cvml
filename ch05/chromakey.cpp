#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void chromakey()
{
	VideoCapture cap1("woman.mp4");
	VideoCapture cap2("raining.mp4");

	if (!cap1.isOpened() || !cap2.isOpened()) {
		cerr << "Video open failed" << endl;

		return;
	}

	int frame_cnt1 = cvRound(cap1.get(CAP_PROP_FRAME_COUNT));
	int frame_cnt2 = cvRound(cap2.get(CAP_PROP_FRAME_COUNT));

	int fps = cap1.get(CAP_PROP_FPS);
	int delay = cvRound(1000 / fps);

	bool do_composit = false;

	Mat frame1;
	Mat frame2;
	Mat hsv;
	Mat mask;
	Mat original;
	Mat filtered;
	while (true)
	{
		cap1 >> frame1;
		if (frame1.empty())
			break;
		cap2 >> frame2;
		if (frame2.empty())
			break;
		frame1.copyTo(original);
		cvtColor(frame1, hsv, COLOR_BGR2HSV);
		inRange(hsv, Scalar(50,50, 0), Scalar(70, 255, 255), mask);
		frame2.copyTo(frame1, mask);
		//bilateralFilter(frame1, filtered, -1, 10, 10);
		if (do_composit == true) {
			imshow("frame", frame1);
			int key = waitKey(delay);
			if (key == 27)
				break;
			if (key == 'q')
				do_composit = false;
		}
		else {
			imshow("frame", original);
			int key = waitKey(delay);
			if (key == 27)
				break;
			if (key == 'q')
				do_composit = true;
		}
		

	}
	cap1.release();
	cap2.release();
	destroyAllWindows();
}
