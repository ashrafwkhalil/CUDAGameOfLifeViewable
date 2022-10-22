# CUDAGameOfLifeViewable
This was a project focused on achieving maximum concurrency and parallelization through developing and scheduling the execution of GPU kernels 
using CUDA.
## Multi-Species Conway's game of life rules
The rules of the original game of life are as follows: If the cell is alive, then it stays alive if it has either 2 or 3 live neighbors. 
If the cell is dead, then it springs to life only in the case that it has 3 live neighbors.
Cells in this case are represented by pixels. The only key difference here is that instead of cells representing simply a binary value, alive or dead, 
while they are alive they are also part of a specific species of alive cells. So now, for a cell to stay alive, it needs 2 or 3 alive neighbors of the same
species, and for a dead cell to come back to life, it needs 3 live neighbors of the same species, and obviously will come alive as that species. Each of 
the different species were represented by different colors. 
## Program Structure Overview
All logic regarding the translation of a current screen state to a future one given the rules of the game were encapsulated completely within a GPU kernel.
The state of the screen was represented by a one dimensional array, meaning that the translation of the one dimensional array into a 2D screen render required
some, albeit minimal, arithmetic gymnastics. Unlike my OpenCL version of this project, the extent of the parallelization here was the kernel executing accross
the compute units of the gpu, and there was no alternation of arrays where one would render to the screen, and the other would act as a buffer. This is explained
further in my OpenCL project. The point here is that the placing of the results of the kernel computations and the rendering of those results happened sequentially,
while in the OpenCl project they happened concurrently. All scheduling was done using simple logic within the main program file. 
## Files
The kernel.cu file includes the kernel for computing the next screen, as well as the logic for calling the kernel function 
correctly within the correct context and with passing the right parameters. This includes using calloc to allocate appropriate portions of 
device memory for the computation, and other important functions necessary to execute to allow for execution of the kernel function. The kernel.h header
file contains some initialized constants and references to the kernel functions as well as the functions that call the kernel functions. The main.cpp file
includes the main method where all pre-defined functions are called.

