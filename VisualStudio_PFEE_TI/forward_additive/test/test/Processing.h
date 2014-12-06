#ifndef PROCESSING_H_
#define PROCESSING_H_

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <string>
#include <stdio.h>
#include <opencv2\highgui\highgui.hpp>
#include "auxfunc.h"
#include "forwadditive.h"
#include "invcomp.h"

class Processing
{
public:
	static int startVideoLK(const char* filename);
	static int startVideoForward(const char* filename);
};

#endif //PROCESSING_H_