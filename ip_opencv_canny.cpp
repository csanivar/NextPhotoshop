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
	double t = (double)getTickCount(); 
	
	namedWindow( "OI" , CV_WINDOW_AUTOSIZE );
 	namedWindow( "PI" , CV_WINDOW_AUTOSIZE );

 	Mat src = imread("great-perhaps.png");
 	Mat dst, gray, abs_dst;
 	
 	// bilateralFilter(src, dst, 15, 80, 80);
 	// GaussianBlur(src, dst, Size(5, 5), 1.0/5.0, 0);
	cvtColor( src, gray, CV_RGB2GRAY );
 	Laplacian( gray, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( dst, abs_dst );
 	imshow("OI", src);
 	imshow("PI", abs_dst);

 	end = clock();
 	elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
 	t = ((double)getTickCount() - t)/getTickFrequency(); 
 	imwrite("opencv_laplacian_out.png", abs_dst);
 	cout<<src.rows<<"x"<<src.cols<<endl;
	return 0;
}