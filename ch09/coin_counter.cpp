#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
void coin_counter();
string type2str(int type);


string type2str(int type)
{
	string r;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	switch (depth)
	{
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}
	return r;
}
void coin_counter()
{
	Mat src = imread("coins1.jpg");

	if (src.empty()) {
		cerr << "File open failed!" << endl;
		return;
	}

	Mat gray, blr;

	cvtColor(src, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, blr, Size(0, 0), 1);

	vector<Vec3f> circles;
	HoughCircles(blr, circles, HOUGH_GRADIENT, 1, 50, 150, 40, 20, 80);

	int sum_of_money = 0;

	Mat dst;
	src.copyTo(dst);

	if (!circles.empty()) {
		for (Vec3f c : circles)
		{
			Point center(c[0], c[1]);
			int radius = c[2];

			circle(dst, center, radius, (0, 0, 255), 1, LINE_AA);
			int x1 = c[0] - radius;
			int y1 = c[1] - radius;
			int x2 = c[0] + radius;
			int y2 = c[1] + radius;
			int w = x2 - x1;
			int h = y2 - y1;
			//Rect는 X, Y, W, H
			Rect crop(x1, y1, w, h);
			Mat cropImage;
			dst.copyTo(cropImage);
			
			//cropImage(crop).copyTo(dst(crop));
			cropImage = cropImage(crop);
			Mat mask = Mat::zeros(cropImage.size(), CV_8U);
			int ch = cropImage.rows;
			int cw = cropImage.cols;
			circle(mask, Size2i(int(cw / 2), int(ch / 2)), radius, 255, -1);

			Mat hsv_total;
			cvtColor(cropImage, hsv_total, COLOR_BGR2HSV);

			Mat hsv[3];
			split(hsv_total, hsv);
			string ty = type2str(hsv[0].type());
			cout << ty.c_str() << endl;
			Mat hue_shift = Mat::zeros(Size(cw, ch), CV_8U);
			// Type 알아맞추는 것이 대단히 중요하다
			for (int i=0; i<hsv[0].rows; i++)
				for (int j=0; j<hsv[0].cols; j++)
					hue_shift.at<uchar>(i,j) = (hsv[0].at<uchar>(i,j) + 40) % 180;
			// Scalar는 기본적으로 더블형 반환
			Scalar mean_of_hue = mean(hue_shift, mask);
			cout << "mean of hue" << mean_of_hue << endl;
			int won = 100;
			if (mean_of_hue[0] < 90)
			{
				won = 10;
			}
			sum_of_money += won;

			putText(cropImage, to_string(won), Point(20, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(255, 0, 0), 2, LINE_AA);
			cropImage.copyTo(dst(crop));
		}
	}

	putText(dst, to_string(sum_of_money) + " won", Point(40, 80), FONT_HERSHEY_DUPLEX, 2, (255, 0, 0), 2, LINE_AA);

	imshow("src", src);
	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}
