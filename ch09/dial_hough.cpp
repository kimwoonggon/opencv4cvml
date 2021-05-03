#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
void dial_hough();
void on_trackbar(int pos, void* userdata);
struct UserData;

struct UserData
{
	Mat src;
	Mat blr;
};

void dial_hough()
{
	//Mat src = imread("dial.jpg");

	struct UserData userdata;
 	userdata.src = imread("dial.jpg");
	
	if (userdata.src.empty())
	{
		cerr << "Image load failed" << endl;
		return;
	}

	Mat gray;
	Mat blr;
	cvtColor(userdata.src, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, blr, Size(0, 0), 1.0);

	userdata.blr = blr;



	namedWindow("img");
	imshow("img", userdata.src);
	createTrackbar("minRadius", "img", 0, 100, on_trackbar, (void*)&userdata);
	createTrackbar("maxRadius", "img", 0, 150, on_trackbar, (void*)&userdata);
	createTrackbar("threshold", "img", 0, 100, on_trackbar, (void*)&userdata);
	
	waitKey();
	destroyAllWindows();

}

void on_trackbar(int pos, void* userdata)
{
	UserData* imginfo = (UserData*)userdata;
	int rmin = getTrackbarPos("minRadius", "img");
	int rmax = getTrackbarPos("maxRadius", "img");
	double th = getTrackbarPos("threshold", "img");

	vector<Vec3f> circles;

	// th, rmin, rmax는 항상 양수여야 함
	HoughCircles(imginfo->blr, circles, HOUGH_GRADIENT, 1, 50, 120, th+1, rmin+1, rmax+1);

	Mat dst;
	(imginfo->src).copyTo(dst);

	if (!circles.empty()) {
		for (Vec3f c : circles)
		{
			int x = (int)c[0];
			int y = (int)c[1];
			int r = (int)c[2];

			circle(dst, Point(x, y), r, (0, 0, 255), 2, LINE_AA);
		}


	}

	imshow("img", dst);

}
