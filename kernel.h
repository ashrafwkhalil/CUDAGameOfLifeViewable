#pragma once
#include "G:\vs\repos\A3426\A3426\Dependencies\glew\include\GL\glew.h" // must be first
#include "G:\vs\repos\A3426\A3426\Dependencies\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <string>
#include <thread>
using namespace std;
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
const int HEIGHT = 768;
const int WIDTH = 1024;

__global__ void computeNextMatrixKernel(int matrix[HEIGHT * WIDTH], int bufferMatrix[HEIGHT * WIDTH]);
cudaError_t doWorkCUDA(int matrix[HEIGHT * WIDTH], int bufferMatrix[HEIGHT * WIDTH], unsigned int size);

