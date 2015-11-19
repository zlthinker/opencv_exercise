#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include <iostream>  
#include <stdlib.h>

using namespace cv;
using namespace std;

//int brightness = 80;
//int contrast = 80;


void adjustContrastAndBrightness();
void splitAndMerge();
void ImageFilter(String);
void ImageFilterBar(String);
void UnlinearFilter(String);
void DilateOrErode(String);
void MorphologicalFunctions(String);
void CannyShow(String);
void SobelShow(String);
void LaplacianShow(String);
void Pyramid(String);
void HoughLinesTest(String);
void HoughLinesPTest(String);
void CircleDetector(String);
void FloodFillTest(String);
void HarrisCornerDetection(String);
void RemapShow(String);
void SurfDectection(String);
void AffineTransformation(String);
void ImageMatcher(String, String);

#endif