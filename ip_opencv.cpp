#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <time.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {
	clock_t start, end;
    double elapsed;
    start = clock();
	namedWindow( "OI" , CV_WINDOW_AUTOSIZE );
 	namedWindow( "PI" , CV_WINDOW_AUTOSIZE );

 	Mat src = imread("great-perhaps.png");
 	Mat dst;

 	// bilateralFilter(src, dst, 15, 80, 80);
 	GaussianBlur(src, dst, Size(5, 5), 1.0/5.0, 0);
 	imshow("OI", src);
 	imshow("PI", dst);
 	end = clock();
 	elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
 	cout<<"Elapsed time: "<<elapsed<<endl;
 	imwrite("PI.png", dst);
 	waitKey(0);
	return 0;
}