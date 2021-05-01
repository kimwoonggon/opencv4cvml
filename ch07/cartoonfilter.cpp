#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat cartoon_filter(const Mat& img);
Mat pencil_sketch(const Mat& img);
void cartoon_filter_start();

Mat cartoon_filter(const Mat& img)
{
	int h, w;
	h = img.rows;
	w = img.cols;

	Mat img2;
	resize(img, img2, Size(w / 2, h / 2));
	Mat blur;
	bilateralFilter(img2, blur, -1, 20, 7);
	Mat edge;
	Canny(img2, edge, 80, 120);
	Mat background(edge.rows, edge.cols, CV_8UC1, Scalar(255));
	edge = background - edge;
	//Mat final_edge;

	cvtColor(edge, edge, COLOR_GRAY2BGR);

	Mat dst;
	bitwise_and(blur, edge, dst);
	Mat final_dst;
	resize(dst, final_dst, Size(w, h), 0, 0, INTER_NEAREST);
	return final_dst;

}

Mat pencil_sketch(const Mat& img)
{
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	Mat blur;
	GaussianBlur(gray, blur, Size(0, 0), 3);
	Mat dst;
	divide(gray, blur, dst, 255);
	return dst;
}

void cartoon_filter_start()
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr << "Camera open failed!" << endl;
		return;
	}

	int cam_mode = 1;
	Mat frame;
	while (true)
	{
		cap >> frame;
		if (frame.empty())
			break;

		if (cam_mode == 1)
		{
			frame = cartoon_filter(frame);

		}
		else if (cam_mode == 2)
		{
			frame = pencil_sketch(frame);
			cvtColor(frame, frame, COLOR_GRAY2BGR);
		}

		imshow("frame", frame);
		int key = waitKey(1);

		if (key == 27)
		{
			break;
		}
		else if (key == 'q')
		{
			cam_mode++;
			if (cam_mode == 3)
			{
				cam_mode = 0;
			}
		}
	}

}
