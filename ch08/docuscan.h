#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

struct MouseParams;
Mat drawROI(const Mat& img, Point2f corner[]);
void on_mouse(int event, int x, int y, int flags, void* param);
void DocScan();

