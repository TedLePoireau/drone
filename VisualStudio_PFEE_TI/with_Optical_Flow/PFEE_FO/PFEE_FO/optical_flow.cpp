#include <opencv2/opencv.hpp>
#include <unordered_map>

using namespace cv;
using namespace std;

Point2f *GetBias(vector<Point2f> *p0, vector<Point2f> *p1, vector<uchar> *st)
{
	Point2f test(0,0);
	int count = 0;
	vector<uchar>::iterator itst = st->begin();
	for (vector<Point2f>::iterator itp0 = p0->end() - 9, itp1 = p1->end() - 9;
		itp0 != p0->end() && itp1 != p1->end() && itst != st->end();
		++itp0, ++itp1, ++itst)
	{
		if (*itst == 1)
		{
			if (itp1->x - itp0->x > test.x - 0.01 && itp1->x - itp0->x < test.x + 0.01 && itp1->y - itp0->y > test.y - 0.01 && itp1->y - itp0->y < test.y + 0.01)
				count++;
			else
				if (count <= 1)
					test = Point2f(itp1->x - itp0->x, itp1->y - itp0->y);
		}
	}
	if (count > 4)
	{
		return &test;
	}
	else
		return &Point2f(0, 0);

}

////
//int main(int argc, char** argv)
//{
//	VideoCapture cap("../../../movingtest720P.mp4"); // open the default camera
//	namedWindow("edges", 1);
//
//	if (!cap.isOpened())  // check if we succeeded
//		return -1;
//
//	//create some random colors
//	vector<Scalar> sc;
//	for (int i = 0; i < 1000; i++)
//	{
//		sc.push_back(Scalar(rand() % 255, rand() % 255, rand() % 255));
//	}
//
//
//	//Take first frame and find corners in it
//	Mat old_frame, old_gray, mask;
//	vector<Point2f> p0;
//	vector<KeyPoint> kp;
//	cap >> old_frame;
//	cvtColor(old_frame, old_gray, CV_BGR2GRAY);
//	FAST(old_gray, kp, 100, true);
//	for (vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it)
//		p0.push_back((*it).pt);
//	//goodFeaturesToTrack(old_gray, p0, 50, 0.01, 7, noArray(), 7);
//
//	//Create a mask image for drawing purposes
//	mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());
//
//	//Coordinate for global disruption
//	Point2f Bias;
//
//
//
//	//Jvais essayer d'afficher point suivis
//	//vector<Point2f> p0;
//	//vector<KeyPoint> kp;
//	//Mat frame, img, frame_gray, mask;
//
//	//	cap >> frame;
//	//	cvtColor(frame, frame_gray, CV_BGR2GRAY);
//	//	//goodFeaturesToTrack(frame_gray, p0, 1000, 0.3, 7, noArray(), 7);
//	//	FAST(frame_gray, kp, 100, true);
//	//	mask = Mat::zeros(frame.rows, frame.cols, frame.type());
//
//	//	printf("nombre de keypoints %d", kp.size());
//	//	for (vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it)
//	//	{
//	//		circle(frame, (*it).pt, 4, sc.at(it - kp.begin()), -1);
//	//	}
//	//	add(frame, mask, img);
//	//	imshow("Tracked Points", img);
//	//}
//
//	//printf("nombre de points initiaux : %d\n", p0.size());
//
//	vector<Point2f> selectedpts;
//
//
//	int count = 0;
//
//	while (true)
//	{
//		Mat frame, frame_gray, err, img;
//		vector<Point2f> p1;
//		vector<uchar> st;
//		if (!(cap.read(frame))) //get one frame form video     
//			break;
//		cvtColor(frame, frame_gray, CV_BGR2GRAY);
//
//
//		//on detecte les nouveaux vehicules de l'image
//		if (count == 5)
//		{
//			count = 0;
//
//			mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());
//			kp.clear();
//			FAST(frame_gray, kp, 100, true);
//			p0.clear();
//			for (vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it)
//				p0.push_back((*it).pt);
//
//			//on introduit des point repères dans l'image
//			int count2 = 0;
//			for (int i = 5; i < frame.rows; i += frame.rows / 3)
//			{
//				for (int j = 5; j < frame.cols; j += frame.cols / 3)
//				{
//					p0.push_back(Point2f(i, j));
//					count2++;
//				}
//			}
//		}
//		else
//			count++;
//
//
//	//	on effectue une transformée de Hough pour savoir ou chercher les points
//	//	Mat dst;
//	//	Canny(frame, dst, 50, 200, 3);
//	//	vector<Vec2f> lines;
//	//	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);
//	//	for (size_t i = 0; i < lines.size(); i++)
//	//	{
//	//		float rho = lines[i][0], theta = lines[i][1];
//	//		Point pt1, pt2;
//	//		double a = cos(theta), b = sin(theta);
//	//		double x0 = a*rho, y0 = b*rho;
//	//		pt1.x = cvRound(x0 + 1000 * (-b));
//	//		pt1.y = cvRound(y0 + 1000 * (a));
//	//		pt2.x = cvRound(x0 - 1000 * (-b));
//	//		pt2.y = cvRound(y0 - 1000 * (a));
//	//		line(mask, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
//	//	}
//
//		////recherche de nouveaux point dans l'image
//		//if (count == 10)
//		//{
//		//	count = 0;
//		//	goodFeaturesToTrack(old_gray, p0, 50, 0.3, 7, noArray(), 7);
//		//}
//		//else
//		//	count++;
//
//
//
//		//calculate optical flow
//		TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 25, 0.03);
//		calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, st, err, cv::Size(15, 15), 1, termcrit);
//		vector<uchar>::iterator itst = st.begin();
//		
//		//Point2f *Bias = GetBias(&p0, &p1, &st);
//
//		//if (Bias->x != 0 || Bias->y != 0)
//		//{
//		//	printf("Bias : \n x : %d \n y %d \n");
//		//}
//
//		selectedpts.clear();
//		for (vector<Point2f>::iterator itp0 = p0.begin(), itp1 = p1.begin();
//			itp0 != p0.end()  && itp1 != p1.end() && itst != st.end();
//			++itp0, ++itp1, ++itst)
//		{
//			if (*itst == 1)
//			{
//				line(mask, *itp1, *itp0, sc.at(itst - st.begin()), 2);
//				circle(frame, *itp1, 5, sc.at(itst - st.begin()), -1);
//				selectedpts.push_back(*itp1);
//			}
//			else
//				selectedpts.push_back(*itp0);
//		}
//
//
//		add(frame, mask, img);
//		
//		
//
//
//		imshow("edges", img);
//
//
//		p0 = selectedpts;
//		frame_gray.copyTo(old_gray);
//		printf("nombre de points trouvés : %d", selectedpts.size());
//	}
//	// the camera will be deinitialized automatically in VideoCapture destructor
//	return 0;
//
//}

template <>
struct std::hash<std::pair<int, int> > {
public:
	size_t operator()(std::pair<int, int> x) const throw() {
		size_t h = (x.first + x.second)*(x.first + x.second + 1)/2 + x.second;   
		return h;
	}
};

//Method 1 : vecteur de déplacement
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step, const Scalar& color) {
	pair<int, int> moveVect;
	unordered_map<pair<int, int>, pair<int,pair<pair<float, float>, pair<float, float>>>> mymap;
	for (int y = 0; y < cflowmap.rows; y += step)
	for (int x = 0; x < cflowmap.cols; x += step)
	{
		const Point2f& fxy = flow.at<Point2f>(y, x);
		moveVect = make_pair(cvRound(fxy.x/10), cvRound(fxy.y/10));
		auto it = mymap.find(moveVect);
		if (it == mymap.end())
		{
			printf("moveVect : %d , %d\n", moveVect.first, moveVect.second);
			mymap.insert(make_pair(moveVect, make_pair(1, make_pair(make_pair(x,y), make_pair(fxy.x, fxy.y)))));
		}
		else
		{
			it->second.first++;
			it->second.second.first.first += x;
			it->second.second.first.second += y;

			it->second.second.second.first += fxy.x;
			it->second.second.second.second += fxy.y;
		}

		////affiche uniquement les object en mouvement
		//if ((fxy.x > 1 || fxy.x < -1) || (fxy.y > 1 || fxy.y < -1))
		//line(cflowmap, Point(x, y), Point(cvRound(x + fxy.x), cvRound(y + fxy.y)),
		//	color);
		//circle(cflowmap, Point(cvRound(x + fxy.x), cvRound(y + fxy.y)), 1, color, -1);
	}

	mymap.erase(make_pair(0, 0));

	float velocity;
	ostringstream ss;
	string toprint;
	for (auto it = mymap.begin(); it != mymap.end(); ++it)
	{
		Point *prev = new Point(it->second.second.first.first / it->second.first, it->second.second.first.second / it->second.first);
		Point *next = new Point(cvRound(prev->x + it->second.second.second.first / it->second.first), cvRound(prev->y + it->second.second.second.second / it->second.first));
		
		//diplay velocity
		velocity = sqrt(pow((next->x - prev->x), 2) + pow((next->y - prev->y), 2));
		ss << velocity;
		toprint = ss.str();
		putText(cflowmap, toprint, *prev, FONT_HERSHEY_SIMPLEX, 0.5, color, 2, 8, false);
		
		line(cflowmap, *prev, *next, color);
	}
	printf("-------------\n");


}

//Method Farneback
int main(int argc, char** argv)
{
	int s = 1;
	//global variables  
	Mat GetImg;
	Mat prvs, next; //current frame  

	char fileName[100] = "../../../movingtest3GP.mp4"; 
	VideoCapture stream1(fileName);     
	if (!(stream1.read(GetImg))) //get one frame form video  
		return 0;
	resize(GetImg, prvs, Size(GetImg.size().width / s, GetImg.size().height / s));
	cvtColor(prvs, prvs, CV_BGR2GRAY);

	//unconditional loop  
	VideoWriter video("out_FarneBack.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(prvs.cols, prvs.rows), true);

	while (true) {

		if (!(stream1.read(GetImg))) //get one frame form video     
			break;
		//Resize  
		resize(GetImg, next, Size(GetImg.size().width / s, GetImg.size().height / s));
		cvtColor(next, next, CV_BGR2GRAY);
		///////////////////////////////////////////////////////////////////  
		Mat flow;
		calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
		Mat cflow;
		cvtColor(prvs, cflow, CV_GRAY2BGR);
		drawOptFlowMap(flow, cflow, 10, CV_RGB(255, 255, 0));
		video.write(cflow);
		imshow("OpticalFlowFarneback", cflow);


		if (waitKey(5) >= 0)
			break;

		prvs = next.clone();

	}
}










//	//Method 2 DENSE OPTICAL FLOW
//	int main(int argc, char** argv)
//	{
//		VideoCapture cap("../../../easy.mp4"); // open the default camera
//		namedWindow("edges", 1);
//
//		if (!cap.isOpened())  // check if we succeeded
//			return -1;
//
//		//create some random colors
//		vector<Scalar> sc;
//		for (int i = 0; i < 1000; i++)
//		{
//			sc.push_back(Scalar(rand() % 255, rand() % 255, rand() % 255));
//		}
//
//
//		//Take first frame and find corners in it
//		Mat old_frame, old_gray, mask;
//		vector<Point2f> p0;
//		cap >> old_frame;
//		cvtColor(old_frame, old_gray, CV_BGR2GRAY);
//
//		//Create a mask image for drawing purposes
//		mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());
//
//
//
//		while (waitKey(30000))
//		{
//
//		}
//		// the camera will be deinitialized automatically in VideoCapture destructor
//		return 0;
//
//}
