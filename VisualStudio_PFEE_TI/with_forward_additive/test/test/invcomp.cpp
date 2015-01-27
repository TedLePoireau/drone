#include <stdio.h>
#include <time.h>
#include <opencv2\openCV.hpp>		// Include header for computer-vision part of OpenCV.
#include <opencv2\highgui\highgui.hpp>	// Include header for GUI part of OpenCV.
#include "auxfunc.h"    // Header for our warping functions.

// Baker-Dellaert-Matthews inverse compositional method
// @param[in] pImgT   Template image T
// @param[in] omega   Rectangular template region
// @param[in] pImgI   Another image I

void align_image_inverse_compositional(IplImage* pImgT, CvRect omega, IplImage* pImgI)
{
	// Some constants for iterative minimization process.
	const float EPS = 1E-5f; // Threshold value for termination criteria.
	const int MAX_ITER = 100;  // Maximum iteration count.

	// Here we will store internally used images.
	IplImage* pGradTx = 0;	   // Gradient of I in X direction.
	IplImage* pGradTy = 0;	   // Gradient of I in Y direction.
	IplImage* pStDesc = 0;		// Steepest descent images.

	// Here we will store matrices.
	CvMat* W = 0;  // Current value of warp W(x,p)
	CvMat* dW = 0;  // Warp update.
	CvMat* idW = 0; // Inverse of warp update.
	CvMat* X = 0;  // Point in coordinate frame of T.
	CvMat* Z = 0;  // Point in coordinate frame of I.

	CvMat* H = 0;  // Hessian.
	CvMat* iH = 0; // Inverse of Hessian.
	CvMat* b = 0;  // Vector in the right side of the system of linear equations.
	CvMat* delta_p = 0; // Parameter update value.

	// Create matrices.
	W = cvCreateMat(3, 3, CV_32F);
	dW = cvCreateMat(3, 3, CV_32F);
	idW = cvCreateMat(3, 3, CV_32F);
	X = cvCreateMat(3, 1, CV_32F);
	Z = cvCreateMat(3, 1, CV_32F);

	H = cvCreateMat(3, 3, CV_32F);
	iH = cvCreateMat(3, 3, CV_32F);
	b = cvCreateMat(3, 1, CV_32F);
	delta_p = cvCreateMat(3, 1, CV_32F);

	// Create images.
	CvSize image_size = cvSize(pImgI->width, pImgI->height);
	pGradTx = cvCreateImage(image_size, IPL_DEPTH_16S, 1);
	pGradTy = cvCreateImage(image_size, IPL_DEPTH_16S, 1);
	pStDesc = cvCreateImage(image_size, IPL_DEPTH_32F, 3);

	// Get current time. We will use it later to obtain total calculation time.
	clock_t start_time = clock();

	/*
	*  Precomputation stage.
	*/

	// Calculate gradient of T.
	cvSobel(pImgT, pGradTx, 1, 0); // Gradient in X direction
	cvConvertScale(pGradTx, pGradTx, 0.125); // Normalize
	cvSobel(pImgT, pGradTy, 0, 1); // Gradient in Y direction
	cvConvertScale(pGradTy, pGradTy, 0.125); // Normalize


	// Compute steepest descent images and Hessian

	cvSet(H, cvScalar(0)); // Set Hessian with zeroes

	int u, v;	// (u,v) - pixel coordinates in the coordinate frame of T.
	float u2, v2; // (u2,v2) - pixel coordinates in the coordinate frame of I.

	// Walk through pixels in the template T.
	int i, j;
	for(i=0; i<omega.width; i++)
	{
		u = i + omega.x;

		for(j=0; j<omega.height; j++)
		{
			v = j + omega.y;

			// Evaluate gradient of T.
			short Tx = CV_IMAGE_ELEM(pGradTx, short, v, u);	
			short Ty = CV_IMAGE_ELEM(pGradTy, short, v, u);	

			// Calculate steepest descent image's element.
			float* stdesc = &CV_IMAGE_ELEM(pStDesc, float, v, u*3); // an element of steepest descent image
			stdesc[0] = (float)(-v*Tx+u*Ty);
			stdesc[1] = (float)Tx;
			stdesc[2] = (float)Ty;

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

	// Invert Hessian.
	double inv_res = cvInvert(H, iH);
	if(inv_res==0)
	{
		printf("Error: Hessian is singular.\n");
		return;
	}

	/*
	*   Iteration stage.
	*/

	// Set warp with identity.
	cvSetIdentity(W);

	// Here we will store current value of mean error.
	float mean_error=0;

	// Iterate
	int iter=0; // number of current iteration
	while(iter < MAX_ITER)
	{
		iter++; // Increment iteration counter

		mean_error = 0; // Set mean error value with zero

		int pixel_count=0; // Count of processed pixels

		cvSet(b, cvScalar(0)); // Set b matrix with zeroes

		// Walk through pixels in the template T.
		int i, j;
		for(i=0; i<omega.width; i++)
		{
			int u = i + omega.x;

			for(j=0; j<omega.height; j++)
			{
				int v = j + omega.y;

				// Set vector X with pixel coordinates (u,v,1)
				SET_VECTOR(X, u, v);

				// Warp Z=W*X
				cvGEMM(W, X, 1, 0, 0, Z);

				// Get coordinates of warped pixel in coordinate frame of I.
				GET_VECTOR(Z, u2, v2);

				// Get the nearest integer pixel coords (u2i;v2i).
				int u2i = cvFloor(u2);
				int v2i = cvFloor(v2);

				if(u2i>=0 && u2i<pImgI->width && // check if pixel is inside I.
					v2i>=0 && v2i<pImgI->height)
				{
					pixel_count++;

					// Calculate intensity of a transformed pixel with sub-pixel accuracy
					// using bilinear interpolation.
					float I2 = interpolate<uchar>(pImgI, u2, v2);

					// Calculate image difference D = I(W(x,p))-T(x).
					float D = I2 - CV_IMAGE_ELEM(pImgT, uchar, v, u);

					// Update mean error value.
					mean_error += fabs(D);

					// Add a term to b matrix.
					float* stdesc = &CV_IMAGE_ELEM(pStDesc, float, v, u*3);
					float* pb = &CV_MAT_ELEM(*b, float, 0, 0);
					pb[0] += stdesc[0] * D;
					pb[1] += stdesc[1] * D;
					pb[2] += stdesc[2] * D;					
				}	
			}
		}

		// Finally, calculate resulting mean error.
		if(pixel_count!=0)
			mean_error /= pixel_count;

		// Find parameter increment. 
		cvGEMM(iH, b, 1, 0, 0, delta_p);
		float delta_wz = CV_MAT_ELEM(*delta_p, float, 0, 0);
		float delta_tx = CV_MAT_ELEM(*delta_p, float, 1, 0);
		float delta_ty = CV_MAT_ELEM(*delta_p, float, 2, 0);

		init_warp(dW, delta_wz, delta_tx, delta_ty);
		// Invert warp.
		inv_res = cvInvert(dW, idW);
		if(inv_res==0)
		{
			printf("Error: Warp matrix is singular.\n");
			return;
		}

		cvGEMM(idW, W, 1, 0, 0, dW);
		cvCopy(dW, W);

		// Print diagnostic information to screen.
		printf("iter=%d mean_error=%f\n", iter, mean_error);

		// Check termination critera.
		if(fabs(delta_wz)<=EPS && fabs(delta_tx)<=EPS && fabs(delta_ty)<=EPS) break;
	}

	// Get current time and obtain total time of calculation.
	clock_t finish_time = clock();
	double total_time = (double)(finish_time-start_time)/CLOCKS_PER_SEC;

	// Print summary.
	printf("===============================================\n");
	printf("Algorithm: inverse compositional.\n");
	printf("Caclulation time: %g sec.\n", total_time);
	printf("Iteration count: %d\n", iter);
	printf("Epsilon: %f\n", EPS);
	printf("Resulting mean error: %f\n", mean_error);
	printf("===============================================\n");

	// Show result of image alignment.
	draw_warped_rect(pImgI, omega, W);
	cvSetImageROI(pImgT, omega);
	cvShowImage("template",pImgT);
	cvShowImage("image",pImgI);
	cvResetImageROI(pImgT);

	// Free used resources and exit.
	cvReleaseMat(&W);
	cvReleaseMat(&dW);
	cvReleaseMat(&idW);
	cvReleaseMat(&H);
	cvReleaseMat(&iH);
	cvReleaseMat(&b);
	cvReleaseMat(&delta_p);
	cvReleaseMat(&X);
	cvReleaseMat(&Z);
}