#include <iostream>
#include <fstream>
#include <Magick++.h>

using namespace std;
using namespace Magick;
typedef unsigned int ui;

int main(int argc, char** argv) {
	ifstream fileInputRed("out_red_g");
	ifstream fileInputGreen("out_green_g");
	ifstream fileInputBlue("out_blue_g");
	
	int dimW;
	int dimH;
	fileInputRed>>dimW;
	fileInputRed>>dimH;
	cout<<dimW<<"x"<<dimH<<endl;
	Image image("great-perhaps.png");
	cout<<image.rows()<<" "<<image.columns()<<endl;

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

	image.write("serial_laplacian_out.png");

	return 0;
}