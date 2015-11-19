#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "functions.h"


using namespace cv;
using namespace std;

int brightness = 80;
int contrast = 80;
int filterVal = 3;
int trackNum = 0;
int g_nMaxIterationNum = 10;
int g_nOpenCloseNum = 0;
int g_nErodeDilateNum = 0;
int g_nTopBlackHatNum = 0;
int g_nElementShape = 0;
Mat img, gray, dst;

static void onTrackbar(int, void*) {
	Mat newImage = Mat::zeros(img.size(), img.type());;
	for (int x = 0; x < img.rows; x++) {
		for (int y = 0; y < img.cols; y++) {
			for (int z = 0; z < 3; z++){
				newImage.at<Vec3b>(x, y)[z] = saturate_cast<char>((contrast*0.01)*img.at<Vec3b>(x, y)[z] + brightness);
			}
		}
	}
	imshow("Window containing trackbar", newImage);
}



void adjustContrastAndBrightness() {
	int thresh_contrast = 160;
	int thresh_bright = 160;
	img = imread("pic.jpg");
	namedWindow("Window containing trackbar", 1);
	createTrackbar("Contrast", "Window containing trackbar", &contrast, 300, onTrackbar);
	createTrackbar("Brightness", "Window containing trackbar", &brightness, 200, onTrackbar);
	onTrackbar(contrast, 0);
	onTrackbar(brightness, 0);
}

void splitAndMerge() {
	Mat img = imread("pic.jpg");
	Mat logo = imread("logo.jpg");
	Mat imgROI = img(Rect(img.cols - logo.cols, img.rows - logo.rows, logo.cols, logo.rows));
	Mat mask = imread("logo.img", 0);
	vector<Mat> channels;
	vector<Mat> logoChannel;
	Mat redChannel;
	Mat redChannelROI;

	split(img, channels);
	split(logo, logoChannel);
	redChannel = channels.at(2);
	redChannelROI = redChannel(Rect(redChannel.cols - logo.cols, redChannel.rows - logo.rows, logo.cols, logo.rows));
	addWeighted(redChannelROI, 1.0, logoChannel.at(2), 0.5, 0, redChannelROI);
	merge(channels, img);


	namedWindow("游戏原画");
	//	namedWindow("logo");
	//	namedWindow("Region of interest");
	namedWindow("Red channel");
	namedWindow("Green channel");
	namedWindow("Blue channel");

	//addWeighted(imgROI, 0.5, logo, 0.8, 0, imgROI);
	//	logo.copyTo(imgROI, mask);

	imshow("游戏原画", img);
	//	imshow("logo", logo);
	//	imshow("Region of interest", imgROI);
	imshow("Blue channel", channels.at(0));
	imshow("Green channel", channels.at(1));
	imshow("Red channel", channels.at(2));
}

static void onFilterBar(int, void*){
	Mat output;
	boxFilter(img, output, -1, Size(filterVal+1, filterVal+1));
	imshow("Filter", output);
}

void ImageFilterBar(String path) {
	img = imread(path);
	namedWindow("Filter");
	createTrackbar("kernel size", "Filter", &filterVal, 40, onFilterBar);
	onFilterBar(filterVal, 0);
	while (char(waitKey(1)) != 'q') {}
}

void ImageFilter(String path){
	Mat src = imread(path);
	Mat output;
	String name = "";
	cout << "Choose type of filter:" << endl
		<< "1: box filter, 2: blur filter, 3: gaussian blur filter, q: exit." << endl;
	char in;
	cin.get(in);
	cout << in << " is choosed." << endl;
	switch (in){
	case '1': boxFilter(src, output, -1, Size(5, 5)); name = "Box filter"; break;
	case '2': blur(src, output, Size(5, 5)); name = "Blur filter"; break;
	case '3': GaussianBlur(src, output, Size(5, 5), 0, 0); name = "Gaussian blur filter"; break;
	case 'q': return;
	default: break;
	}
	namedWindow(name);
	imshow(name, output);
	waitKey(10000);
}

/**************Task 9**************/
void onUnlinearFilterChange(int, void*){
	Mat out;
//	medianBlur(img, out, filterVal*2+1);
	bilateralFilter(img, out, filterVal, filterVal*2, filterVal/2);
	imshow("Unlinear Filter", out);
}

void UnlinearFilter(String path) {
	img = imread(path);
	namedWindow("Unlinear Filter");
	createTrackbar("filter", "Unlinear Filter", &filterVal, 40, onUnlinearFilterChange);
	onUnlinearFilterChange(filterVal, 0);
	cout << "Press q to exit.\n";
	while ((char)waitKey(1) != 'q') {}
}

static void onDilateOrErode(int, void*){
	Mat element = getStructuringElement(MORPH_RECT, Size(2 * filterVal + 1, 2 * filterVal + 1), Point(filterVal, filterVal));
	Mat out;
	if (trackNum == 0){
		dilate(img, out, element);
	}
	else {
		erode(img, out, element);
	}
	imshow("Dilate/Erode", out);
}

void DilateOrErode(String path) {
	img = imread(path);
	namedWindow("Dilate/Erode");
	imshow("Dilate/Erode", img);
	createTrackbar("a", "Dilate/Erode", &trackNum, 1, onDilateOrErode);
	createTrackbar("b", "Dilate/Erode", &filterVal, 21, onDilateOrErode);
	onDilateOrErode(filterVal, 0);
	cout << "Press q to exit.\n";
	while ((char)waitKey(1) != 'q') {}
}

/**********task 11************/
void onOpenClose(int, void*){
	int offset = g_nOpenCloseNum - g_nMaxIterationNum;
	int absoOffset = abs(offset);
	Mat element = getStructuringElement(g_nElementShape, Size(absoOffset*2+1, absoOffset*2+1), Point(absoOffset, absoOffset));
	Mat out;
	if (offset < 0)
		morphologyEx(img, out, CV_MOP_OPEN, element);
	else
		morphologyEx(img, out, CV_MOP_CLOSE, element);
	imshow("开运算/闭运算", out);
}

void onErodeDilate(int, void*){
	int offset = g_nErodeDilateNum - g_nMaxIterationNum;
	int absoOffset = abs(offset);
	Mat element = getStructuringElement(g_nElementShape, Size(absoOffset * 2 + 1, absoOffset * 2 + 1), Point(absoOffset, absoOffset));
	Mat out;
	if (offset < 0)
		morphologyEx(img, out, CV_MOP_ERODE, element);
	else
		morphologyEx(img, out, CV_MOP_DILATE, element);
	imshow("膨胀/腐蚀", out);
}

void onTopBlackHat(int, void*){
	int offset = g_nTopBlackHatNum - g_nMaxIterationNum;
	int absoOffset = abs(offset);
	Mat element = getStructuringElement(g_nElementShape, Size(absoOffset * 2 + 1, absoOffset * 2 + 1), Point(absoOffset, absoOffset));
	Mat out;
	if (offset < 0)
		morphologyEx(img, out, MORPH_TOPHAT, element);
	else
		morphologyEx(img, out, MORPH_BLACKHAT, element);
	imshow("顶帽/黑帽", out);
}

void MorphologicalFunctions(String path) {
	img = imread(path);
	g_nOpenCloseNum = 9;
	g_nErodeDilateNum = 9;
	g_nTopBlackHatNum = 2;

	namedWindow("开运算/闭运算");
	namedWindow("膨胀/腐蚀");
	namedWindow("顶帽/黑帽");

	createTrackbar("open/close", "开运算/闭运算", &g_nOpenCloseNum, g_nMaxIterationNum * 2 + 1, onOpenClose);
	createTrackbar("dilate/erode", "膨胀/腐蚀", &g_nErodeDilateNum, g_nMaxIterationNum * 2 + 1, onErodeDilate);
	createTrackbar("top/black hat", "顶帽/黑帽", &g_nTopBlackHatNum, g_nMaxIterationNum * 2 + 1, onTopBlackHat);

	printf("\n\n\n\t请调整滚动条观察图像效果~\n\n");
	printf("\n\n\t按键操作说明: \n\n"
		"\t\t键盘按键【ESC】或者【Q】- 退出程序\n"
		"\t\t键盘按键【1】- 使用椭圆(Elliptic)结构元素\n"
		"\t\t键盘按键【2】- 使用矩形(Rectangle )结构元素\n"
		"\t\t键盘按键【3】- 使用十字型(Cross-shaped)结构元素\n"
		"\t\t键盘按键【空格SPACE】- 在矩形、椭圆、十字形结构元素中循环\n"
		);

	while (1){
		onOpenClose(g_nOpenCloseNum, 0);
		onErodeDilate(g_nErodeDilateNum, 0);
		onTopBlackHat(g_nTopBlackHatNum, 0);

		char c = waitKey(0);
		if (c == 'q' || c == 27)
			break;
		if (c == '1')
			g_nElementShape = MORPH_ELLIPSE;
		else if (c == '2')
			g_nElementShape = MORPH_CROSS;
		else if (c == '3')
			g_nElementShape = MORPH_RECT;
		else if (c == ' ')
			g_nElementShape = (g_nElementShape + 1) % 3;
	}
	
	return;
}

void CannyShow(String path) {
	img = imread(path);
	Mat img1 = img.clone();

	Canny(img, img, 150, 100, 3);
	imshow("Canny边缘检测(黑白)", img);

	Mat dst, edge, gray;
	dst.create(img1.size(), img1.type());
	cvtColor(img1, gray, CV_BGR2GRAY);
//	imshow("gray", gray);

	blur(gray, edge, Size(3, 3));   //edge stores image after blur filter, it's more ambiguous
//	imshow("edge", edge);
	Canny(edge, edge, 3, 9, 3);    // edge stores edges of image
//	imshow("edge after canny", edge);

	dst = Scalar::all(0);
	img1.copyTo(dst, edge);  //edge acts as mask that indicates which elements need to be copied
	imshow("dst", dst);

	waitKey(0);
}

void SobelShow(String path) {
	Mat grad_x, grad_y, gray;
	Mat abs_grad_x, abs_grad_y, dst;

	img = imread(path);

	Sobel(img, grad_x, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(img, grad_y, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	imshow("Sobel", dst);

	waitKey(0);
}

void LaplacianShow(String path) {
	img = imread(path);
	Mat gray, dst, abs_dst;
	GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
	cvtColor(img, gray, CV_RGB2GRAY);
	imshow("gray", gray);
	Laplacian(gray, dst, CV_16S, 3);
	imshow("dst", dst);
	convertScaleAbs(dst, abs_dst);
	imshow("Laplacian", abs_dst);

	waitKey(0);
}

/*******task 13********/
void Pyramid(String path) {
	Mat src = imread(path);
	Mat dst;
	imshow("Original", src);
	pyrDown(src, dst, Size(0.5*src.rows, 0.5*src.cols));
	imshow("pyDown", dst);
	pyrUp(dst, dst, Size(src.rows, src.cols));
	imshow("pyUp", dst);
	waitKey(0);
}

/*********task 14*********/
void HoughLinesTest(String path) {
	Mat src, dst, gray;
	src = imread(path);

	Canny(src, gray, 50, 100);
	cvtColor(gray, dst, CV_GRAY2BGR);
	namedWindow("dst", WINDOW_NORMAL);
	imshow("dst", dst);

	vector<Vec2f> lines;
	HoughLines(gray, lines, 1, CV_PI/180, 150);

	for (int i = 0; i < lines.size(); i++){
		float rho = lines[i][0];
		float theta = lines[i][1];
		float x = cos(theta);
		float y = sin(theta);
		Point2d p1, p2;
		p1.x = cvRound(rho * x - 100 * y);
		p1.y = cvRound(rho * y + 100 * x);
		p2.x = cvRound(rho * x + 100 * y);
		p2.y = cvRound(rho * y - 100 * x);
		line(dst, p1, p2, Scalar(255, 0, 0), 1, CV_AA);
	}
	namedWindow("lines", WINDOW_NORMAL);
	imshow("lines", dst);
	waitKey(0);
}

void HoughLinesPTest(String path) {
	Mat src, dst, gray;
	src = imread(path);

	Canny(src, gray, 50, 100);
	cvtColor(gray, dst, CV_GRAY2BGR);
//	namedWindow("dst", WINDOW_NORMAL);
//	imshow("dst", dst);

	vector<Vec4i> lines;
	HoughLinesP(gray, lines, 1, CV_PI / 180, 160, 100, 20);

	for (int i = 0; i < lines.size(); i++){
		Vec4i l = lines[i];
		line(dst, Point2d(l[0], l[1]), Point2d(l[2], l[3]), Scalar(255, 0, 0), 1, CV_AA);
	}
	namedWindow("lines", WINDOW_NORMAL);
	imshow("lines", dst);
	waitKey(0);
}

void CircleDetector(String path) {
	Mat src, mid, dst;
	src = imread(path);

	cvtColor(src, mid, CV_BGR2GRAY);
	GaussianBlur(mid, mid, Size(9, 9), 2, 2);

	vector<Vec3f> circles;   //must be Vec3f, Vec3d is wrong
	HoughCircles(mid, circles, CV_HOUGH_GRADIENT, 2, 10, 200);

	for (int i = 0; i < circles.size(); i++) {
		Vec3d cir = circles[i];
		Point p = Point(cvRound(cir[0]), cvRound(cir[1]));
		int radius = cvRound(cir[2]);
		circle(src, p, radius, Scalar(0, 255, 0), 3);
	}

	namedWindow("CircleDetector", WINDOW_NORMAL);
	imshow("CircleDetector", src);
	waitKey(0);
}

/**************task 15*****************/
static void onMouse(int event, int x, int y, int, void*){
	if (event != CV_EVENT_LBUTTONDOWN)
		return;
	
	Point seed = Point(x, y);
//	cout << seed << "is clicked.\n";
	int flag = 8 | FLOODFILL_MASK_ONLY | FLOODFILL_FIXED_RANGE | (38 << 8);

	int b = (unsigned)theRNG() & 255;
	int g = (unsigned)theRNG() & 255;
	int r = (unsigned)theRNG() & 255;
	Scalar color = Scalar(b, g, r);

	Mat mask;
	mask.create(img.rows + 2, img.cols + 2, CV_8UC1);

	int number = floodFill(dst, seed, color, 0, Scalar(20, 20, 20), Scalar(20, 20, 20));
	imshow("FloodFill", dst);
	cout << number << " pixels are filled.\n";
}

void FloodFillTest(String path) {
	img = imread(path);
	img.copyTo(dst);
	cvtColor(img, gray, CV_BGR2GRAY);

	namedWindow("FloodFill", WINDOW_NORMAL);
	setMouseCallback("FloodFill", onMouse, 0);
	imshow("FloodFill", dst);

	waitKey(0);

}

/*********task 16   Harris corner detection******************/
int gl_harris = 30;

void onHarrisCorner(int, void*) {
	Mat gray, dst;
	Mat src_copy = img.clone();
	dst = Mat::zeros(img.size(), CV_32FC1);
	cvtColor(img, gray, CV_BGR2GRAY);
	cornerHarris(gray, dst, 2, 3, 0.04);
	normalize(dst, dst, 0, 255, NORM_MINMAX, CV_32FC1);

	vector<Vec2i> corners;
	goodFeaturesToTrack(gray, corners, 25, 0.01, 10);

	for (int i = 0; i < corners.size(); i++) {
		circle(src_copy, corners[i], 10, Scalar(255, 0, 0), 2);
	}

	for (int i = 0; i < dst.rows; i++) {
		for (int j = 0; j < dst.cols; j++) {
			if (dst.at<float>(i, j) > gl_harris + 80) {
				circle(src_copy, Point(j, i), 5, Scalar(0, 0, 255), 2);
			}
		}

	}
	imshow("Harris Corner", src_copy);
}
void HarrisCornerDetection(String path) {
	img = imread(path);

	namedWindow("Harris Corner", CV_WINDOW_AUTOSIZE);
	createTrackbar("threshold", "Harris Corner", &gl_harris, 175, onHarrisCorner);
	onHarrisCorner(0, 0);
	waitKey(0);
}

/**************task 17***************/
void RemapShow(String path) {
	Mat src, dst;
	Mat map_x, map_y;
	src = imread(path);
	dst.create(src.size(), src.type());
	map_x.create(src.size(), CV_32FC1);
	map_y.create(src.size(), CV_32FC1);
	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			map_x.at<float>(i, j) = static_cast<float>(j);
			map_y.at<float>(i, j) = static_cast<float>(src.rows - i);
			dst.at<float>(i, j) = src.at<float>(i, j);
		}
	}
	remap(src, dst, map_x, map_y, CV_INTER_LINEAR);
	imshow("After remap", dst);
	waitKey(0);
}

void SurfDectection(String path) {
	Mat src, dst;
	src = imread(path);

	int minHessian = 400; //min hessian value of SURF
	SurfFeatureDetector detector(minHessian);
	vector<KeyPoint> keypoints;
	detector.detect(src, keypoints);
	drawKeypoints(src, keypoints, dst, Scalar::all(-1), 4);
  //last parameter = 4, draw circles whose radius correspond to keypoint size
	imshow("surf", dst);
	waitKey();
}

/******************task 18**********************/
void AffineTransformation(String path) {
	Mat src = imread(path);
	Mat dst;
	Point2f srcTriangle[3];
	Point2f dstTriangle[3];
	Mat rotMat(2, 3, CV_32FC1);
	Mat warpMat(2, 3, CV_32FC1);

	/**** warp transform ********/
	srcTriangle[0] = Point2f(0, 0);
	srcTriangle[1] = Point2f(static_cast<float>(src.cols - 1), 0);
	srcTriangle[2] = Point2f(0, static_cast<float>(src.rows - 1));

	dstTriangle[0] = Point2f(static_cast<float>(src.cols*0.0), static_cast<float>(src.rows*0.33));
	dstTriangle[1] = Point2f(static_cast<float>(src.cols*0.65), static_cast<float>(src.rows*0.35));
	dstTriangle[2] = Point2f(static_cast<float>(src.cols*0.15), static_cast<float>(src.rows*0.6));

	warpMat = getAffineTransform(srcTriangle, dstTriangle);
	warpAffine(src, dst, warpMat, src.size());
	imshow("warp", dst);

	/********* rotation *****************/
	Point center = Point(dst.rows / 2, dst.cols / 2);
	double angle = -30;
	double scale = 1;
	rotMat = getRotationMatrix2D(center, angle, scale);
	warpAffine(dst, dst, rotMat, src.size());

	imshow("Affine", dst);
	waitKey();
}

void ImageMatcher(String path1, String path2) {
	Mat src1, src2, dst;
	src1 = imread(path1);
	src2 = imread(path2);

	int minHessian = 800;         //400-800 works well
	SurfFeatureDetector detector(minHessian);
	vector<KeyPoint> keypoints1, keypoints2;
	Mat ROI = src1(Rect(0, 0, src1.cols/2, src1.rows/2));
	detector.detect(ROI, keypoints1);
	detector.detect(src2, keypoints2);

	SurfDescriptorExtractor extractor;
	Mat descriptor1, descriptor2;
	extractor.compute(src1, keypoints1, descriptor1);
	extractor.compute(src2, keypoints2, descriptor2);

	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	matcher.match(descriptor1, descriptor2, matches);

	Mat imgMatches;
	drawMatches(src1, keypoints1, src2, keypoints2, matches, dst);
	namedWindow("After match", WINDOW_NORMAL);
	imshow("After match", dst);

	waitKey(0);
}