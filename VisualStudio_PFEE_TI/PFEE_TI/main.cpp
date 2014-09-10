#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class findroad{                              //class that separates out roads from images
private:
	Mat path;
public:
	void setpath(Mat& image)
	{
		image.convertTo(path, CV_32S);
	}

	Mat getroad(Mat &image)                  //major working function attribute of the class
	{
		watershed(image, path);                                     //using watershed segmenter
		path.convertTo(path,CV_8U);
		return path;
	}
};


int main(int argc, char** argv)
{
	string filename = "C:/Users/Benjamin/Documents/Visual Studio 2010/Projects/PFEE_TI/road.MP4";
	VideoCapture capture(filename);
	Mat frame, gray, Erode, Dilate;

	if( !capture.isOpened() )
		throw "Error when reading steam_avi";

	namedWindow("founded road", 1);
	namedWindow("input image", 1);

	for( ; ; )
	{
		capture >> frame;

		if(frame.empty())
			break;

		cvtColor(frame, gray, CV_BGR2GRAY);
		threshold(gray, gray, 100, 255, THRESH_BINARY);
		double t = 0;
		t = (double)cvGetTickCount();
		erode(gray, Erode, Mat(), Point(2,2), 7);
		dilate(gray, Dilate, Mat(), Point(2,2), 7);
		threshold(Dilate, Dilate, 1, 50, THRESH_BINARY_INV);
		Mat path_trace(gray.size(), CV_8U, Scalar(0));
		path_trace = Erode + Dilate;
		findroad road;
		road.setpath(path_trace);
		Mat road_found = road.getroad(frame);
		road_found.convertTo(road_found, CV_8U);
		imshow("founded road", road_found);
		imshow("input image", frame);
		t = (double) cvGetTickCount() - t;
		waitKey(20); // waits to display frame
	}
	waitKey(0); // key press to close window
	// releases and window destroy are automatic in C++ interface
	return 0;

}