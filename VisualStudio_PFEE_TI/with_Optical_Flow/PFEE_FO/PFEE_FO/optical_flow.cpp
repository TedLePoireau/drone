#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	VideoCapture cap("../../../road.MP4"); // open the default camera
	namedWindow("edges",1);

	if(!cap.isOpened())  // check if we succeeded
		return -1;

	//create some random colors
	Scalar c(rand() % 255, rand() % 255, rand() % 255);	

	//Take first frame and find corners in it
	Mat old_frame, old_gray, mask;
	vector<Point2f> p0;
	cap >> old_frame;
	cvtColor(old_frame, old_gray, CV_BGR2GRAY); 
	goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, noArray(), 7);

	//Create a mask image for drawing purposes
	mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());

	for(;;)
	{
		Mat frame, frame_gray, err, img;
		vector<Point2f> p1;
		vector<uchar> st;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, frame_gray, CV_BGR2GRAY);

		//calculate optical flow
		calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, st, err, cv::Size(15,15), 2);
		vector<uchar>::iterator itst = st.begin();

		for (vector<Point2f>::iterator itp0 = p0.begin(), itp1 = p1.begin();
			itp0 != p0.end() && itp1 != p1.end() && itst != st.end();
			++itp0, ++itp1, ++itst)
		{
			if (*itst == 1)
			{
				line(mask, *itp1, *itp0, c, 2);
				circle(frame, *itp1, 5, c, -1);
			}
		}

		add(frame, mask, img);

		imshow("edges", img);
		if(waitKey(30) >= 0) break;

		old_gray.copyTo(frame_gray);
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}