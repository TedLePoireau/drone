/*

///////////////////////ROAD DETECTION WITH WATERSHED

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

} */

////////////////////////////////// VEHICLE DETECTIOJN

#include <stdio.h>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui_c.h>

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

void detect(IplImage *img);

int main(int argc, char** argv)
{
  CvCapture *capture;
  IplImage  *frame;
  int input_resize_percent = 100;
  
  if(argc < 3)
  {
    std::cout << "Usage " << argv[0] << " cascade.xml video.avi" << std::endl;
    return 0;
  }

  if(argc == 4)
  {
    input_resize_percent = atoi(argv[3]);
    std::cout << "Resizing to: " << input_resize_percent << "%" << std::endl;
  }

  cascade = (CvHaarClassifierCascade*) cvLoad(argv[1], 0, 0, 0);
  storage = cvCreateMemStorage(0);
  capture = cvCaptureFromAVI(argv[2]);

  assert(cascade && storage && capture);

  cvNamedWindow("video", 1);

  IplImage* frame1 = cvQueryFrame(capture);
  frame = cvCreateImage(cvSize((int)((frame1->width*input_resize_percent)/100) , (int)((frame1->height*input_resize_percent)/100)), frame1->depth, frame1->nChannels);

  const int KEY_SPACE  = 32;
  const int KEY_ESC    = 27;

  int key = 0;
  do
  {
    frame1 = cvQueryFrame(capture);

    if(!frame1)
      break;

    cvResize(frame1, frame);

    detect(frame);

    key = cvWaitKey(10);

    if(key == KEY_SPACE)
      key = cvWaitKey(0);

    if(key == KEY_ESC)
      break;

  }while(1);

  cvDestroyAllWindows();
  cvReleaseImage(&frame);
  cvReleaseCapture(&capture);
  cvReleaseHaarClassifierCascade(&cascade);
  cvReleaseMemStorage(&storage);

  return 0;
}

void detect(IplImage *img)
{
  CvSize img_size = cvGetSize(img);
  CvSeq *object = cvHaarDetectObjects(
    img,
    cascade,
    storage,
    1.1, //1.1,//1.5, //-------------------SCALE FACTOR
    1, //2        //------------------MIN NEIGHBOURS
    0, //CV_HAAR_DO_CANNY_PRUNING
    cvSize(0,0),//cvSize( 30,30), // ------MINSIZE
    img_size //cvSize(70,70)//cvSize(640,480)  //---------MAXSIZE
    );

  std::cout << "Total: " << object->total << " cars" << std::endl;
  for(int i = 0 ; i < ( object ? object->total : 0 ) ; i++)
  {
    CvRect *r = (CvRect*)cvGetSeqElem(object, i);
    cvRectangle(img,
      cvPoint(r->x, r->y),
      cvPoint(r->x + r->width, r->y + r->height),
      CV_RGB(255, 0, 0), 2, 8, 0);
  }

  cvShowImage("video", img);
}