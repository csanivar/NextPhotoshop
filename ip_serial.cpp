#include <iostream>
#include <fstream>
#include <time.h>

#define FW 3
#define FH 3

typedef unsigned int ui;
using namespace std;

int filter[5][5] = 
{
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1,
};

double filterGaussian[5][5] = 
{
	1.0/16.0, 4.0/16.0, 6.0/16.0, 4.0/16.0, 1.0/16.0,
	4.0/16.0, 16.0/16.0, 24.0/16.0, 16.0/16.0, 4.0/16.0,
	6.0/16.0, 24.0/16.0, 36.0/16.0, 24.0/16.0, 6.0/16.0,
	4.0/16.0, 16.0/16.0, 24.0/16.0, 16.0/16.0, 4.0/16.0,
	1.0/16.0, 4.0/16.0, 6.0/16.0, 4.0/16.0, 1.0/16.0,
};

double filterFindHorEdges[5][5] =
{
   0,  0, -1,  0,  0,
   0,  0, -1,  0,  0,
   0,  0,  2,  0,  0,
   0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
};

double filterLaplacian[3][3] = 
{
	0, 1, 0,
	1, -4, 1,
	0, 1, 0
};

double factor = 1.0;
double bias = 0.0;

double factorGaussian = 1.0/5.0;

int main(int argc, char** argv) {
	ifstream fileRed("red");
	ifstream fileGreen("green");
	ifstream fileBlue("blue");
	ofstream fileOutRed("out_red_g");
	ofstream fileOutGreen("out_green_g");
	ofstream fileOutBlue("out_blue_g");
	clock_t start, end;
    double elapsed;
    start = clock();

	int dimW;
	int dimH; 
	fileRed>>dimW;
	fileRed>>dimH;
	
	ui red[dimW][dimH];
	ui green[dimW][dimH];
	ui blue[dimW][dimH];
	ui resultRed[dimW][dimH];
	ui resultGreen[dimW][dimH];
	ui resultBlue[dimW][dimH];

	for(int i=0; i<dimW; i++) {
		for(int j=0; j<dimH; j++) {
			fileRed>>red[i][j];
			fileGreen>>green[i][j];
			fileBlue>>blue[i][j];
		}
	}

	for(int x=0; x<dimW; x++) {
		for(int y=0; y<dimH; y++) {
			ui redSum = 0;
			ui greenSum = 0;
			ui blueSum = 0;

			for(int fX=0; fX<FW; fX++) {
				for(int fY=0; fY<FH; fY++) {
					int imageX = (x - FW/2  + fX + dimW) % dimW; 
					int imageY = (y - FH/2  + fY + dimH) % dimH;
					redSum += red[imageX][imageY] * filterLaplacian[fX][fY];
					greenSum += green[imageX][imageY] * filterLaplacian[fX][fY];
					blueSum += blue[imageX][imageY] * filterLaplacian[fX][fY];
				}
			}
			resultRed[x][y] = factor * redSum + bias;
			resultGreen[x][y] = factor * greenSum + bias;
			resultBlue[x][y] = factor * blueSum + bias;
		}
	}

	fileOutRed<<dimW<<" "<<dimH<<endl;

	for(int i=0; i<dimW; i++) {
		for(int j=0; j<dimH; j++) {
			fileOutRed<<resultRed[i][j]<<" ";
			fileOutGreen<<resultGreen[i][j]<<" ";
			fileOutBlue<<resultBlue[i][j]<<" ";
		}
	}
	end = clock();
 	elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
 	cout<<"Elapsed time: "<<elapsed<<endl;
	return 0;
}