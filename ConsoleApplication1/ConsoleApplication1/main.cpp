#include "functions.h"

using namespace cv;
using namespace std;


int main()
{
	system("color 5E");
//	adjustContrastAndBrightness();
//	DilateOrErode("me.jpg");
//	MorphologicalFunctions("pic.jpg");
//	AbstractSIFT("me.jpg", "pic.jpg");
//	CannyShow("pic.jpg");
//	SobelShow("pic.jpg");
//	LaplacianShow("pic.jpg");
//	Pyramid("pyramid.jpg");
//	HoughLinesPTest("horse.jpg");
//	CircleDetector("circles.png");
//	FloodFillTest("logo.jpg");
//	HarrisCornerDetection("black_and_white.jpg");
//	RemapShow("me.jpg");
//	SurfDectection("rectan.png");
//	AffineTransformation("pic.jpg");
	ImageMatcher("rect.jpg", "rectan.png");

	return 0;
}

