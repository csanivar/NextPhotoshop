#include <iostream>
#include <fstream>

#define FW 5
#define FH 5

typedef unsigned int ui;
using namespace std;

int filter[FW][FH] = 
{
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1,
};

double factor = 1.0/13.0;
double bias = 0.0;

int main(int argc, char** argv) {
	ifstream fileRed("red");
	ifstream fileGreen("green");
	ifstream fileBlue("blue");
	ofstream fileOutRed("out_red");
	ofstream fileOutGreen("out_green");
	ofstream fileOutBlue("out_blue");

	const int dimW = 600;
	const int dimH = 600; 
	int temp;
	fileRed>>temp>>temp;
	fileGreen>>temp>>temp;
	fileBlue>>temp>>temp;
	
	ui red[dimW][dimH];
	ui green[dimW][dimH];
	ui blue[dimW][dimH];

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
					redSum += red[imageX][imageY] * filter[fX][fY];
					greenSum += green[imageX][imageY] * filter[fX][fY];
					blueSum += blue[imageX][imageY] * filter[fX][fY];
				}
			}
			red[x][y] = factor * redSum + bias;
			green[x][y] = factor * greenSum + bias;
			blue[x][y] = factor * blueSum + bias;
		}
	}


	for(int i=0; i<dimW; i++) {
		for(int j=0; j<dimH; j++) {
			fileOutRed<<red[i][j]<<" ";
			fileOutGreen<<green[i][j]<<" ";
			fileOutBlue<<blue[i][j]<<" ";
		}
	}

	cout<<"Dimenstions are: "<<dimW<<" "<<dimH<<endl;
	return 0;
}