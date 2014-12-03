#include <stdio.h>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include "auxfunc.h"
#include "forwadditive.h"
#include "invcomp.h"

// Our plan:
// 
// 1. Ask user to enter image warp parameter vector p=(wz, tx, ty, s)
// 2. Define our template rectangle to be bounding rectangle of the butterfly
// 3. Warp image I with warp matrix W(p)
// 4. Show template T and image T, wait for a key press
// 5. Estimate warp parameters using Lucas-Kanade method, wait for a key press
// 6. Estimate warp parameters using Baker-Dellaert-Matthews, wait for a key press
//

using namespace cv;

int main(int argc, char* argv[])
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

	return 0;
}


