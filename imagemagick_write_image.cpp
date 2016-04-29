#include <iostream>
#include <fstream>
#include <Magick++.h>

using namespace std;
using namespace Magick;
typedef unsigned int ui;

int main(int argc, char** argv) {
	ifstream fileInputRed("out_red");
	ifstream fileInputGreen("out_green");
	ifstream fileInputBlue("out_blue");
	Image image("great-perhaps.png");
	
	const int dimW=600;
	const int dimH=600;

	for(int i=0; i<dimW; i++) {
		for(int j=0; j<dimH; j++) {
			ui red, green, blue;
			fileInputRed>>red;
			fileInputGreen>>green;
			fileInputBlue>>blue;
			Color newColor = Color(red, green, blue);
			image.pixelColor(i, j, newColor);
		}
	}

	image.write("output_image.png");

	return 0;
}