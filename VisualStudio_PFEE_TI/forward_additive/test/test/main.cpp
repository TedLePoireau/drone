#include "Processing.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Processing::startVideoLK("../data/movingtest720P.mp4");
	//Processing::startVideoForward("../data/movingtest720P.mp4");

	// the camera will be deinitialized automatically in VideoCapture destructor
	system("pause");
	return 0;
}