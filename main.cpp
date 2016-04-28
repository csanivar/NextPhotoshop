#include <iostream>
#include <Magick++.h>

using namespace std;
using namespace Magick;
typedef unsigned int ui;

ui dimX, dimY;

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