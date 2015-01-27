#include "Processing.h"

using namespace cv;
using namespace std;

int Processing::startVideoLK(const char* filename)
{
	VideoCapture cap(filename); // open the default camera
	namedWindow("edges", 1);

	if (!cap.isOpened())  // check if we succeeded
		return -1;

	//create some random colors
	vector<Scalar> sc;

	for (int i = 0; i < 1000; i++)
	{
		sc.push_back(Scalar(rand() % 255, rand() % 255, rand() % 255));
	}

	//Take first frame and find corners in it
	Mat old_frame, old_gray, mask;
	vector<Point2f> p0;
	vector<KeyPoint> kp;

	cap >> old_frame;
	cvtColor(old_frame, old_gray, CV_BGR2GRAY);
	FAST(old_gray, kp, 100, true);

	for (vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it)
		p0.push_back((*it).pt);

	//Create a mask image for drawing purposes
	mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());

	vector<Point2f> selectedpts;

	int count = 0;
	int nbframe = 0;
	float first = -1.0f;
	Point2f pprec, psave, tmp; 

	while (true)
	{
		Mat frame, frame_gray, err, img;
		vector<Point2f> p1;
		vector<uchar> st;

		if (!(cap.read(frame))) //get one frame form video     
			break;

		cvtColor(frame, frame_gray, CV_BGR2GRAY);

		//on detecte les nouveaux vehicules de l'image
		if (count == 5)
		{
			count = 0;
			mask = Mat::zeros(old_frame.rows, old_frame.cols, old_frame.type());

			kp.clear();
			p0.clear();

			FAST(frame_gray, kp, 100, true);

			for (vector<KeyPoint>::iterator it = kp.begin(); it != kp.end(); ++it)
				p0.push_back((*it).pt);

			//on introduit des point repères dans l'image
			int count2 = 0;

			for (int i = 5; i < frame.rows; i += frame.rows / 3)
			{
				for (int j = 5; j < frame.cols; j += frame.cols / 3)
				{
					p0.push_back(Point2f(i, j));
					count2++;
				}
			}
		}
		else
			count++;

		//calculate optical flow
		TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 25, 0.03);

		calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, st, err, cv::Size(15, 15), 1, termcrit);

		vector<uchar>::iterator itst = st.begin();

		selectedpts.clear();

		/*
		Declaration de vmax;
		*/
		float vmax = 0, vspeed;
		Point2f pspeed;

		for (vector<Point2f>::iterator itp0 = p0.begin(), itp1 = p1.begin();
			itp0 != p0.end()  && itp1 != p1.end() && itst != st.end();
			++itp0, ++itp1, ++itst)
		{
			if (*itst == 1)
			{
				//line(mask, *itp1, *itp0, sc.at(itst - st.begin()), 2);
				//circle(frame, *itp1, 5, sc.at(itst - st.begin()), -1);

				vspeed = float(sqrt(float((itp1->x - itp0->x) * (itp1->x - itp0->x)
					+ (itp1->y - itp0->y) * (itp1->y - itp0->y)))) ;

				/*
				recuperation de la vitesse la plus rapide
				et du point correspondant
				*/
				if (vspeed > vmax)
				{
					vmax = vspeed;
					pspeed = *itp1;
				}

				if (*itp1 == psave)
					psave = *itp1;

				selectedpts.push_back(*itp1);
			}
			else
				selectedpts.push_back(*itp0);
		}


		/*
		Si le pt de la vitesse max correspond au pt de l'image
		precedent alors on sauvegarde ce point
		*/
		if ((first == -1.0f) ||
			(pprec.x <= 50) ||
			(pprec.y <= 50))
		{
			pprec = pspeed;
			psave = pprec;
			first = 10.0f;
		}
		else
		{
			if (pprec == pspeed)
				pprec = pspeed;
			else
				pprec = psave;
			
			/*
			permet de changer d'encadrement
			si le cadre reste sur bloque dans le vide
			si on commente ces lignes l'encadrement risque
			de suivre un vehicule jusqu'à se mettre hors champs.
			*/
			int n = abs(cvRound(pprec.x - tmp.x)) + abs(cvRound(pprec.y - tmp.y));

			if (n < 10)
			{
				pprec = pspeed;
				psave = pprec;
			}
		}

		tmp = pprec;

		//on encadre le pt precedent 
		rectangle(frame, Rect(pprec.x - 25, pprec.y -25, 50, 50), Scalar(255),1, 8,0);
		add(frame, mask, img);

		imshow("edges", img);
		p0 = selectedpts;
		frame_gray.copyTo(old_gray);
		//		printf("nombre de points trouvés : %d \n", selectedpts.size());

		if (waitKey(5) >= 0)
			break;

		nbframe++;

		cout << endl;
	}
}

int Processing::startVideoForward(const char* filename)
{
	// Ask user to enter image warp paremeters vector.
	// p = (wz, tx, ty),

	float WZ=-0.01, TX=5, TY=-3;

	// Here we will store our images.
	IplImage* pColorPhoto = 0; // Photo of a butterfly on a flower.
	IplImage* pGrayPhoto = 0;  // Grayscale copy of the photo.
	IplImage* pImgT = 0;	   // Template T.
	IplImage* pImgI = 0;	   // Image I.

	// Here we will store our warp matrix.
	CvMat* W = 0;  // Warp W(p)
	Mat getImg;

	// Create two simple windows. 
	cvNamedWindow("template"); // Here we will display T(x).
	cvNamedWindow("image");

	char fileName[100] = "../data/movingtest3GP.mp4"; 
	CvCapture* cap = cvCaptureFromFile(fileName);


	if (!(pColorPhoto = cvQueryFrame(cap)))
		return 0;

	// Create other images.
	CvSize photo_size = cvSize(pColorPhoto->width,pColorPhoto->height);
	pGrayPhoto = cvCreateImage(photo_size, IPL_DEPTH_8U, 1);
	pImgT = cvCreateImage(photo_size, IPL_DEPTH_8U, 1);
	pImgI = cvCreateImage(photo_size, IPL_DEPTH_8U, 1);

	// Convert photo to grayscale, because we need intensity values instead of RGB.	
	cvCvtColor(pColorPhoto, pGrayPhoto, CV_RGB2GRAY);

	// Create warp matrix, we will use it to create image I.
	W = cvCreateMat(3, 3, CV_32F);

	// Set region of interest to butterfly's bounding rectangle.
	CvRect omega = cvRect(200, 100, 50, 50);

	// Create template T
	cvCopy(pGrayPhoto, pImgT);

	init_warp(W, WZ, TX, TY);
	warp_image(pGrayPhoto, pImgI, W);

	cvSetIdentity(W);
	draw_warped_rect(pImgI, omega, W);

	cvSetImageROI(pImgT, omega);
	cvShowImage("image", pImgI);
	cvShowImage("template", pImgI);
	cvResetImageROI(pImgT);

	while (true)
	{
		if (!(pColorPhoto = cvQueryFrame(cap))) //get one frame form video  
			break;

		cvCvtColor(pColorPhoto, pGrayPhoto, CV_RGB2GRAY);
		init_warp(W, WZ, TX, TY);
		warp_image(pGrayPhoto, pImgI, W);

		align_image_forwards_additive(pImgT, omega, pImgI);

		if (cvWaitKey(5) >= 0)
			break;
	}

	// Release all used resources and exit
	cvDestroyWindow("template");
	cvDestroyWindow("image");
	cvReleaseImage(&pImgT);
	cvReleaseImage(&pImgI);
	cvReleaseMat(&W);
}

bool operator==(Point2f p1, Point2f p2)
{
	int n = (abs(cvRound(p1.x - p2.x)) + abs(cvRound(p1.y - p2.y)));

	return n <= 50;
}

