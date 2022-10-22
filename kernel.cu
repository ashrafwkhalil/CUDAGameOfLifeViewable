#include "G:\vs\repos\A3426\A3426\Dependencies\glew\include\GL\glew.h" // must be first
#include "G:\vs\repos\A3426\A3426\Dependencies\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <string>
#include <thread>
using namespace std;
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include "kernel.h"
const int size = 600;

cudaError_t addWithCuda(int *c, const int *a, const int *b, unsigned int size);
cudaError_t doWorkCUDA(int matrix[HEIGHT * WIDTH], int bufferMatrix[HEIGHT * WIDTH], unsigned int sizee);
void initMatrix(int matrix[HEIGHT * WIDTH], int size);

__global__ void addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}
__device__ int xCompute(int i)
{
    return i % WIDTH;
}
__device__ int yCompute(int i)
{
    return i / WIDTH;
}
__device__ int xyCompute(int i, int j)
{
    return (j * WIDTH) + i;
}

__global__ void computeNextMatrixKernel(int matrix[HEIGHT * WIDTH], int bufferMatrix[HEIGHT * WIDTH])
{

    int i = blockIdx.x * blockDim.x + threadIdx.x;

    int surround[8];
    if (xCompute(i) > 0 && yCompute(i) > 0)
    {
        surround[0] = matrix[xyCompute(xCompute(i) - 1, yCompute(i) - 1)];
    }
    else
        surround[0] = 0;
    if (yCompute(i) > 0)
        surround[1] = matrix[xyCompute(xCompute(i), yCompute(i) - 1)];
    else
        surround[1] = 0;
    if (xCompute(i) < WIDTH && yCompute(i) > 0)
        surround[2] = matrix[xyCompute(xCompute(i) + 1, yCompute(i) - 1)];
    else
        surround[2] = 0;
    if (xCompute(i) < WIDTH)
        surround[3] = matrix[xyCompute(xCompute(i) + 1, yCompute(i))];
    else
        surround[3] = 0;
    if (xCompute(i) < WIDTH && yCompute(i) < HEIGHT)
        surround[4] = matrix[xyCompute(xCompute(i) + 1, yCompute(i) + 1)];
    else
        surround[4] = 0;
    if (yCompute(i) < HEIGHT)
        surround[5] = matrix[xyCompute(xCompute(i), yCompute(i) + 1)];
    else
        surround[5] = 0;
    if (xCompute(i) > 0 && yCompute(i) < HEIGHT)
        surround[6] = matrix[xyCompute(xCompute(i) - 1, yCompute(i) + 1)];
    else
        surround[6] = 0;
    if (xCompute(i) > 0)
        surround[7] = matrix[xyCompute(xCompute(i) - 1, yCompute(i))];
    else
        surround[7] = 0;

    int live[4];
    for (int ii = 0; ii < 4; ii++)
    {
        int counter = 0;
        for (int i = 0; i < 8; i++)
        {
            if (surround[i] == ii + 1)
            {
                counter++;
            }
        }
        live[ii] = counter;
    }
    if (matrix[i] != 0)
    {

        if (live[matrix[i] - 1] < 2)
        {

            bufferMatrix[i] = 0;
        }
        else if ((live[matrix[i] - 1] == 2) || (live[matrix[i] - 1] == 3))
        {

            bufferMatrix[i] = matrix[i];
        }
        else if (live[matrix[i] - 1] > 3)
        {

            bufferMatrix[i] = 0;
        }
    }
    else if (matrix[i] == 0)
    {
        int c = 0;
        int d = 0;
        for (int jj = 0; jj < 4; jj++)
        {
            if (live[jj] == 3)
            {
                c = jj;
                d = 1;
                break;
            }
        }
        if (d == 1)
            bufferMatrix[i] = c + 1;
        else
            bufferMatrix[i] = 0;
    }
}

cudaError_t doWorkCUDA(int matrix[HEIGHT * WIDTH], int bufferMatrix[HEIGHT * WIDTH], unsigned int size)
{

    int *dev_matrix = 0;
    int *dev_bufferMatrix = 0;
    cudaError_t cudaStatus;

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    cudaStatus = cudaMalloc((void **)&dev_matrix, size * sizeof(int));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void **)&dev_bufferMatrix, size * sizeof(int));
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_matrix, matrix, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy to device failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_bufferMatrix, bufferMatrix, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy to device failed!");
        goto Error;
    }

    computeNextMatrixKernel<<<768, 1024>>>(dev_matrix, dev_bufferMatrix);

    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching nextMatrix!\n", cudaStatus);
        goto Error;
    }

    cudaStatus = cudaMemcpy(bufferMatrix, dev_bufferMatrix, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess)
    {
        fprintf(stderr, "cudaMemcpy to host failed!");
        goto Error;
    }

Error:
    cudaFree(dev_matrix);
    cudaFree(dev_bufferMatrix);

    return cudaStatus;
}
