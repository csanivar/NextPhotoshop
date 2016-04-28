#include <iostream>
#include <Magick++.h>

#define FH 5
#define FW 5
using namespace std;
using namespace Magick;
typedef unsigned int ui;

ui dimX, dimY;
double factor = 1.0/9.0;
double bias = 0.0;
int filter[FW][FH] = 
{
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1,
};


int main(int argc, char *argv[]) {
	InitializeMagick(*argv);
	
	Image image("great-perhaps.png");
	dimX = image.rows();
	dimY = image.columns();

	ui red[dimX][dimY];
	ui green[dimX][dimY];
	ui blue[dimX][dimY];
	ui alpha[dimX][dimY];

	//Read image pixel data into contructed arrays
	for(int i=0;i<dimX; i++) {
		for(int j=0; j<dimY; j++) {
			Color pixel = image.pixelColor(i, j);
			red[i][j] = pixel.redQuantum();
			green[i][j] = pixel.greenQuantum();
			blue[i][j] = pixel.blueQuantum();
			alpha[i][j] = pixel.alphaQuantum();
		}
	}


	for(int x=0; x<dimX; x++) {
		for(int y=0; y<dimY; y++) {
			ui redSum = 0;
			ui greenSum = 0;
			ui blueSum = 0;

			for(int fX=0; fX<FW; fX++) {
				for(int fY=0; fY<FH; fY++) {
					int imageX = (x - FW/2  + fX + dimX) % dimX; 
					int imageY = (y - FH/2  + fY + dimY) % dimY;
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


	for(int i=0; i<dimX; i++) {
		for(int j=0; j<dimY; j++) {
			Color newColor = Color(red[i][j], green[i][j], blue[i][j]);
			image.pixelColor(i, j, newColor);
		}
	}

	image.write("out.png");

  	cout<<"Image dimensions: "<<dimX<<"x"<<dimY<<endl;
    return 0;
}