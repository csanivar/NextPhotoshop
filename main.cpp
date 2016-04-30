#include <iostream>
#include <Magick++.h>
#include <fstream>

#define FH 3
#define FW 3
using namespace std;
using namespace Magick;
typedef unsigned int ui;

ui dimX, dimY;
double factor = 1.0/13.0;
double bias = 0.0;
/*int filter[FW][FH] = 
{
	1, 0, 0, 0, 0,
	0, 1, 0, 0, 0,
	0, 0, 1, 0, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 0, 1,
};*/

int filter[FW][FH] = 
{
	-1, -1, -1,
	-1, 9, -1,
	-1, -1, -1
};

/*__global__ void convoluteOnDevice(ui* dRed, ui* dGreen, ui* dBlue, ui* dFilter,
						ui* resultRed, ui* resultGreen, ui* resultBlue, int dimX, int dimY) {
    ui valRed = 0;
    ui valGreen = 0;
    ui valBlue = 0;

    unsigned int row = blockIdx.y * BLOCK_SIZE + threadIdx.y;
    unsigned int col = blockIdx.x * BLOCK_SIZE + threadIdx.x;

    if (row > N || col > N) {
        return;
    }

    for (int i = 0; i < FW; i++) {
    	for(int j=0; j < FH; j++) {
    		int imageX = (row - FW/2  + i + dimX) % dimX; 
			int imageY = (col - FH/2  + fY + dimY) % dimY;
			valRed += dRed[imageX * dimX + imageY] * filter[i * FW + j];
			valGreen += dGreen[imageX * dimX + imageY] * filter[i * FW + j];
			valBlue += dBlue[imageX * dimX + imageY] * filter[i * FW + j];
    	}
    }

    resultRed[row * dimX + col] = factor * valRed + bias;
    resultGreen[row * dimX + col] = factor * valGreen + bias;
    resultBlue[row * dimX + col] = factor * valBlue + bias;
}*/


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

	ofstream fileRed("red");
	ofstream fileGreen("green");
	ofstream fileBlue("blue");

	fileRed<<dimX<<" "<<dimY<<endl;

	for(int i=0; i<dimX; i++) {
		for(int j=0; j<dimY; j++) {
			fileRed<<red[i][j]<<" ";
			fileGreen<<green[i][j]<<" ";
			fileBlue<<blue[i][j]<<" ";
		}
		fileRed<<endl;
		fileGreen<<endl;
		fileBlue<<endl;
	}

	/*for(int x=0; x<dimX; x++) {
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
	}*/

    /*cudaMemcpy(red, resultRed, dimX * dimY * sizeof(ui), cudaMemcpyDeviceToHost);
    cudaMemcpy(green, resultGreen, dimX * dimY * sizeof(ui), cudaMemcpyDeviceToHost);
    cudaMemcpy(blue, resultBlue, dimX * dimY * sizeof(ui), cudaMemcpyDeviceToHost);

    cudaFree(dRed);
    cudaFree(dGreen);
    cudaFree(dBlue);
    cudaFree(dFilter);
    cudaFree(resultRed);
    cudaFree(resultGreen);
    cudaFree(resultBlue);

	for(int i=0; i<dimX; i++) {
		for(int j=0; j<dimY; j++) {
			Color newColor = Color(red[i][j], green[i][j], blue[i][j]);
			image.pixelColor(i, j, newColor);
		}
	}*/

	image.write("find-edges.png");

  	// cout<<"Image dimensions: "<<dimX<<"x"<<dimY<<endl;
    return 0;
}