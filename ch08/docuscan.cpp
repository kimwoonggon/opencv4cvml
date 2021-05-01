#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

struct MouseParams;
Mat drawROI(const Mat& img, Point2f corner[]);
void onmouse(int event, int x, int y, int flags, void* param);
void DocScan();

struct MouseParams
{
	Mat src = imread("scanned.jpg");

	int h = src.rows;
	int w = src.cols;
	int dh = 500;
	int dw = cvRound(dh * 290 / 210);
	Point2f srcQuad[4] = { Point2f(30,30), Point2f(30, h - 30), Point2f(w - 30,h - 30), Point2f(w - 30,30) };
	Point2f dstQuad[4] = { Point2f(0,0), Point2f(0,dh - 1), Point2f(dw - 1,dh - 1), Point2f(dw - 1, 0) };
	Point2f ptOld = Point2f(30, 30);
	bool dragSrc[4] = { false, false, false, false };
};


Mat drawROI(const Mat& img, Point2f corner[])
{
	Mat cpy;
	img.copyTo(cpy);

	Scalar c1(192, 192, 255);
	Scalar c2(128, 128, 255);

	for (int i = 0; i < 4; i++)
	{
		circle(cpy, corner[i], 25, c1, -1, LINE_AA);
	}

	line(cpy, corner[0], corner[1], c2, 2, LINE_AA);
	line(cpy, corner[1], corner[2], c2, 2, LINE_AA);
	line(cpy, corner[2], corner[3], c2, 2, LINE_AA);
	line(cpy, corner[3], corner[0], c2, 2, LINE_AA);

	Mat disp;
	addWeighted(img, 0.3, cpy, 0.7, 0, disp);

	return disp;
}

void onmouse(int event, int x, int y, int flags, void* param)
{
	MouseParams* mp = (MouseParams*)param;
	if (event == EVENT_LBUTTONDOWN)
	{
		for (int i = 0; i < 4; i++)
		{
			if (norm(mp->srcQuad[i] - Point2f(x, y)) < 25)
			{

				mp->dragSrc[i] = true;
				mp->ptOld.x = float(x);
				mp->ptOld.y = float(y);

				break;
			}
		}
	}

	if (event == EVENT_LBUTTONUP)
	{
		for (int i = 0; i < 4; i++)
		{
			mp->dragSrc[i] = false;
		}
	}

	if (event == EVENT_MOUSEMOVE)
	{
		for (int i = 0; i < 4; i++)
		{
			if (mp->dragSrc[i])
			{
				cout << "drag true" << endl;
				Point2f diff((float)x-mp->ptOld.x, (float)y-mp->ptOld.y);

				mp->srcQuad[i] = mp->srcQuad[i] + diff;
				Mat cpy;
				cpy = drawROI(mp->src, mp->srcQuad);
				imshow("img", cpy);
				mp->ptOld.x = float(x);
				mp->ptOld.y = float(y);
				break;
			}
		}
	}
}


void DocScan()
{
	MouseParams mp;
	Mat disp;
	disp = drawROI(mp.src, mp.srcQuad);

	imshow("img", disp);
	setMouseCallback("img", onmouse, (void*)&mp);

	while (true)
	{
		int key = waitKey();
		if (key == 13)
		{
			break;
		}
		if (key == 27)
		{
			destroyWindow("img");
			return;
		}
	}

	Mat pers;
	pers = getPerspectiveTransform(mp.srcQuad, mp.dstQuad);
	Mat dst;
	warpPerspective(mp.src, dst, pers, Size(mp.dw, mp.dh), INTER_CUBIC);

	imshow("dst", dst);
	waitKey();
	destroyAllWindows();
}
