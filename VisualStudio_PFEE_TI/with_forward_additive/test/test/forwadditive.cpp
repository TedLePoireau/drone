#include <stdio.h>
#include <time.h>
#include <opencv2\openCV.hpp>			// Include header for computer-vision part of OpenCV.
#include <opencv2\highgui\highgui.hpp>	// Include header for GUI part of OpenCV.
#include "auxfunc.h"    // Header for our warping functions.

// Lucas-Kanade method
// @param[in] pImgT   Template image T
// @param[in] omega   Rectangular template region
// @param[in] pImgI   Another image I

void align_image_forwards_additive(IplImage* pImgT, CvRect omega, IplImage* pImgI)
{
	// Some constants for iterative minimization process.
	const float EPS = 1E-5f; // Threshold value for termination criteria.
	const int MAX_ITER = 100;  // Maximum iteration count.

	// Here we will store internally used images.
	IplImage* pGradIx = 0;	   // Gradient of I in X direction.
	IplImage* pGradIy = 0;	   // Gradient of I in Y direction.

	// Here we will store matrices.
	CvMat* W = 0;  // Current value of warp W(x,p)
	CvMat* X = 0;  // Point in coordinate frame of T.
	CvMat* Z = 0;  // Point in coordinate frame of I.

	CvMat* H = 0;  // Hessian
	CvMat* iH = 0; // Inverse of Hessian
	CvMat* b = 0;  // Vector in the right side of the system of linear equations.
	CvMat* delta_p = 0; // Parameter update value.

	// Create matrices.
	W = cvCreateMat(3, 3, CV_32F);
	X = cvCreateMat(3, 1, CV_32F);
	Z = cvCreateMat(3, 1, CV_32F);

	H = cvCreateMat(3, 3, CV_32F);
	iH = cvCreateMat(3, 3, CV_32F);
	b = cvCreateMat(3, 1, CV_32F);
	delta_p = cvCreateMat(3, 1, CV_32F);

	// Create gradient images.
	CvSize image_size = cvSize(pImgI->width,pImgI->height);
	pGradIx = cvCreateImage(image_size, IPL_DEPTH_16S, 1);
	pGradIy = cvCreateImage(image_size, IPL_DEPTH_16S, 1);

	// Get current time. We will use it later to obtain total calculation time.
	clock_t start_time = clock();

	/*
	*  Precomputation stage.
	*/

	// Calculate gradient of I.
	cvSobel(pImgI, pGradIx, 1, 0); // Gradient in X direction
	cvConvertScale(pGradIx, pGradIx, 0.125); // Normalize
	cvSobel(pImgI, pGradIy, 0, 1); // Gradient in Y direction
	cvConvertScale(pGradIy, pGradIy, 0.125); // Normalize

	/*
	*  Iteration stage.
	*/

	// Here we will store parameter approximation. 
	float wz_a=0, tx_a=0, ty_a=0;

	// Here we will store current mean error value.
	float mean_error=0;

	// Iterate
	int iter=0; // number of current iteration
	while(iter < MAX_ITER)
	{
		iter++; // Increment iteration counter

		mean_error = 0; // Set mean error value with zero

		int pixel_count=0; // Count of processed pixels

		init_warp(W, wz_a, tx_a, ty_a); // Init warp W(x, p)
		cvSet(H, cvScalar(0)); // Set Hessian with zeroes
		cvSet(b, cvScalar(0)); // Set b matrix with zeroes


		// (u,v) - pixel coordinates in the coordinate frame of T.
		int u, v;

		// Walk through pixels in the template T.
		int i, j;
		for(i=0; i<omega.width; i++)
		{
			u = i + omega.x;

			for(j=0; j<omega.height; j++)
			{
				v = j + omega.y;

				// Set vector X with pixel coordinates (u,v,1)
				SET_VECTOR(X, u, v);

				// Warp Z=W*X
				cvGEMM(W, X, 1, 0, 0, Z);

				// (u2,v2) - pixel coordinates in the coordinate frame of I.
				float u2, v2;

				// Get coordinates of warped pixel in coordinate frame of I.
				GET_VECTOR(Z, u2, v2);

				// Get the nearest integer pixel coords (u2i;v2i).
				int u2i = cvFloor(u2);
				int v2i = cvFloor(v2);

				if(u2i>=0 && u2i<pImgI->width && // check if pixel is inside I.
					v2i>=0 && v2i<pImgI->height)
				{
					pixel_count++;

					// Evaluate gradient of I at W(x,p) with subpixel accuracy
					// using bilinear interpolation.
					float Ix = interpolate<short>(pGradIx, u2, v2);
					float Iy = interpolate<short>(pGradIy, u2, v2);

					// Calculate steepest descent image's element.
					float stdesc[3]; // an element of steepest descent image
					stdesc[0] = (float)(-v*Ix+u*Iy);
					stdesc[1] = (float)Ix;
					stdesc[2] = (float)Iy;

					// Calculate intensity of a transformed pixel with sub-pixel accuracy
					// using bilinear interpolation.
					float I2 = interpolate<uchar>(pImgI, u2, v2);

					// Calculate image difference D = T(x)-I(W(x,p)).
					float D = CV_IMAGE_ELEM(pImgT, uchar, v, u) - I2;

					// Update mean error value.
					mean_error += fabs(D);

					// Add a term to b matrix.
					float* pb = &CV_MAT_ELEM(*b, float, 0, 0);
					pb[0] += stdesc[0] * D;
					pb[1] += stdesc[1] * D;
					pb[2] += stdesc[2] * D;

					// Add a term to Hessian.
					int l,m;
					for(l=0;l<3;l++)
					{
						for(m=0;m<3;m++)
						{
							CV_MAT_ELEM(*H, float, l, m) += stdesc[l]*stdesc[m];
						}
					}
				}	
			}
		}

		// Finally, calculate mean error.
		if(pixel_count!=0)
			mean_error /= pixel_count;

		// Invert Hessian.
		double inv_res = cvInvert(H, iH);
		if(inv_res==0)
		{
			//printf("Error: Hessian is singular.\n");
			return;
		}

		// Find parameter increment. 
		cvGEMM(iH, b, 1, 0, 0, delta_p);
		float delta_wz = CV_MAT_ELEM(*delta_p, float, 0, 0);
		float delta_tx = CV_MAT_ELEM(*delta_p, float, 1, 0);
		float delta_ty = CV_MAT_ELEM(*delta_p, float, 2, 0);

		// Update parameter vector approximation.
		wz_a += delta_wz;
		tx_a += delta_tx;
		ty_a += delta_ty;

		// Print diagnostic information to screen.
		//printf("iter=%d dwz=%f dtx=%f dty=%f mean_error=%f\n", 
		//	iter, delta_wz, delta_tx, delta_ty, mean_error);

		// Check termination critera.
		if(fabs(delta_wz)<EPS && fabs(delta_tx)<EPS && fabs(delta_ty)<EPS) break;
	}

	// Get current time and obtain total time of calculation.
	clock_t finish_time = clock();
	double total_time = (double)(finish_time-start_time)/CLOCKS_PER_SEC;

	/*
	// Print summary.
	printf("===============================================\n");
	printf("Algorithm: forward additive.\n");
	printf("Caclulation time: %g sec.\n", total_time);
	printf("Iteration count: %d\n", iter);
	printf("Approximation: wz_a=%f tx_a=%f ty_a=%f\n", wz_a, tx_a, ty_a);
	printf("Epsilon: %f\n", EPS);
	printf("Resulting mean error: %f\n", mean_error);
	printf("===============================================\n");
	*/

	// Show result of image alignment.
	init_warp(W, wz_a, tx_a, ty_a);
	draw_warped_rect(pImgI, omega, W);
	cvSetImageROI(pImgT, omega);
	cvShowImage("template",pImgT);
	cvShowImage("image",pImgI);
	cvResetImageROI(pImgT);

	// Free used resources and exit.
	cvReleaseMat(&W);
	cvReleaseMat(&X);
	cvReleaseMat(&Z);

	cvReleaseMat(&H);
	cvReleaseMat(&iH);
	cvReleaseMat(&b);
	cvReleaseMat(&delta_p);
}