#include <iostream>
#include <fstream>
#include "device_launch_parameters.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "cuda_device_runtime_api.h"

#define FW 5
#define FH 5
#define BLOCK_SIZE 50
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

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

double filterFindHorEdges[FW][FH] =
{
   0,  0, -1,  0,  0,
   0,  0, -1,  0,  0,
   0,  0,  2,  0,  0,
   0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
};

double filterFindVerEdges[FW][FH] =
{
   0,  0, -1,  0,  0,
   0,  0, -1,  0,  0,
   0,  0,  4,  0,  0,
   0,  0, -1,  0,  0,
   0,  0, -1,  0,  0
}


double factor = 1.0/13.0;
double bias = 0.0;

static void HandleError( cudaError_t err,
                         const char *file,
                         int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
                file, line );
        exit( EXIT_FAILURE );
    }
}

__global__ void convoluteOnDevice(ui* dRed, ui* dGreen, ui* dBlue, ui* dFilter,
						ui* resultRed, ui* resultGreen, ui* resultBlue, int dimX, int dimY, double factor, double bias) {
    ui valRed = 0;
    ui valGreen = 0;
    ui valBlue = 0;

    unsigned int row = blockIdx.y * BLOCK_SIZE + threadIdx.y;
    unsigned int col = blockIdx.x * BLOCK_SIZE + threadIdx.x;

    if (row > dimW || col > dimH) {
        return;
    }

    for (int i = 0; i < FW; i++) {
    	for(int j=0; j < FH; j++) {
    		int imageX = (row - FW/2  + i + dimW % dimW; 
			int imageY = (col - FH/2  + jo + dimH) % dimH;
			valRed += dRed[imageX * dimW + imageY] * filter[i * FW + j];
			valGreen += dGreen[imageX * dimW + imageY] * filter[i * FW + j];
			valBlue += dBlue[imageX * dimW + imageY] * filter[i * FW + j];
    	}
    }

    resultRed[row * dimX + col] = factor * valRed + bias;
    resultGreen[row * dimX + col] = factor * valGreen + bias;
    resultBlue[row * dimX + col] = factor * valBlue + bias;
}

int main(int argc, char** argv) {
	ifstream fileRed("red");
	ifstream fileGreen("green");
	ifstream fileBlue("blue");
	ofstream fileOutRed("cuda_out_red");
	ofstream fileOutGreen("cuda_out_green");
	ofstream fileOutBlue("cuda_out_blue");

	const int dimW = 600;
	const int dimH = 600; 
	int temp;
	fileRed>>temp;
	fileRed>>temp;
	
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

	/*for(int x=0; x<dimW; x++) {
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
	}*/

	//CUDA helper start
	ui* dRed;
	ui* dGreen;
	ui* dBlue;
	ui* resultRed;
	ui* resultGreen;
	ui* resultBlue;
	ui* dFilter;
		
	cudaMalloc((void**) &dRed, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &dGreen, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &dBlue, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &dFilter, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &resultRed, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &resultGreen, dimW * dimH * sizeof(ui));
    cudaMalloc((void**) &resultBlue, dimW * dimH * sizeof(ui));

    cudaMemcpy(dRed, red, dimW * dimH * sizeof(ui), cudaMemcpyHostToDevice);
    cudaMemcpy(dGreen, green, dimW * dimH * sizeof(ui), cudaMemcpyHostToDevice);
    cudaMemcpy(dBlue, blue, dimW * dimH * sizeof(ui), cudaMemcpyHostToDevice);
    cudaMemcpy(dFilter, filter, FW * FH * sizeof(ui), cudaMemcpyHostToDevice);

    im3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid(dimW/dimBlock.x, dimH/dimBlock.y);

    convoluteOnDevice<<<dimBlock, dimGrid>>>(dRed, dGreen, dBlue, resultRed, resultGreen, resultBlue, dimW, dimH, factor, bias);
    cudaThreadSychronize();

    cudaMemcpy(red, resultRed, dimW * dimH * sizeof(ui), cudaMemcpyDeviceToHost);
    cudaMemcpy(green, resultGreen, dimW * dimH * sizeof(ui), cudaMemcpyDeviceToHost);
    cudaMemcpy(blue, resultBlue, dimW * dimH * sizeof(ui), cudaMemcpyDeviceToHost);

    cudaFree(dRed);
    cudaFree(dGreen);
    cudaFree(dBlue);
    cudaFree(dFilter);
    cudaFree(resultRed);
    cudaFree(resultGreen);
    cudaFree(resultBlue);
	//CUDA helper end

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