#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void movie_transition()
{
	VideoCapture cap1("video1.mp4");
	VideoCapture cap2("video2.mp4");

	if (!cap1.isOpened() || !cap2.isOpened()) {
		cerr << "Video open failed" << endl;
		return;
	}

	cout << "Frame 1 width: " << cvRound(cap1.get(CAP_PROP_FRAME_WIDTH)) << endl;
	cout << "Frame 1 height: " << cvRound(cap1.get(CAP_PROP_FRAME_HEIGHT)) << endl;
	cout << "Frame 2 width: " << cvRound(cap2.get(CAP_PROP_FRAME_WIDTH)) << endl;
	cout << "Frame 2 height: " << cvRound(cap2.get(CAP_PROP_FRAME_HEIGHT)) << endl;

	// We assume that FPS and Size of two movies are same.
	int frame_cnt1 = cvRound(cap1.get(CAP_PROP_FRAME_COUNT));
	int frame_cnt2 = cvRound(cap2.get(CAP_PROP_FRAME_COUNT));
	int fps1 = cap1.get(CAP_PROP_FPS);
	int fps2 = cap2.get(CAP_PROP_FPS);
	int delay = cvRound(1000 / fps1);

	cout << "FPS1 : " << fps1 << endl;
	cout << "FPS2 : " << fps2 << endl;
	
	int effect_frames = fps1 * 2;
	cout << "FRAME 1 COUNT : " << frame_cnt1 << endl;
	cout << "FRAME 2 COUNT : " << frame_cnt2 << endl;

	int w = cvRound(cap1.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(cap1.get(CAP_PROP_FRAME_HEIGHT));

	int fourcc = VideoWriter::fourcc('M', 'P', 'E', 'G');
	VideoWriter outputVideo("output.mp4", fourcc, fps1, Size(w, h));

	if (!outputVideo.isOpened()) {
		cout << "File open failed!" << endl;
		return;
	}

	// 두 동영상 이어 붙이기
	Mat frame1;
	Mat frame2;
	
	for (int i=0; i<frame_cnt1 - effect_frames; i++) {
		cap1 >> frame1;
		if (frame1.empty())
			break;
		outputVideo << frame1;
		imshow("frame", frame1);
		
		if (waitKey(delay) == 27)
			break;

	}
	// 합성구간
	for (int i = 1; i < effect_frames; i++) {
		cap1 >> frame1;
		cap2 >> frame2;
		if (frame2.empty() || frame1.empty())
			break;
		
		int dx = w * i / effect_frames;
		Mat frame(h, w, frame1.type(), Scalar(0,0,0));
		Rect frame1_roi(dx, 0, w-dx, h);
		Rect frame2_roi(0, 0, dx, h);

		frame2(frame2_roi).copyTo(frame(frame2_roi));
		frame1(frame1_roi).copyTo(frame(frame1_roi));
		
		outputVideo << frame;
		imshow("frame", frame);
		if (waitKey(delay) == 27)
			break;
	}

	for (int i = effect_frames; i < frame_cnt2; i++) {
		cap2 >> frame2;
		if (frame2.empty())
			break;
		outputVideo << frame2;
		imshow("frame", frame2);

		if (waitKey(delay) == 27)
			break;

	}



}
