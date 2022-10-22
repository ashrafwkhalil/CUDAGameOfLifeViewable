#include <vector>
#include <iostream>
#include <ctime>
#include <cstdint>
#include <stdio.h>
#ifdef _WIN32
#  define WINDOWS_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#endif
#include <helper_gl.h>
#if defined (_APPLE_) || defined(MACOSX)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#include <GLUT/glut.h>
#ifndef glutCloseFunc
#define glutCloseFunc glutWMCloseFunc
#endif
#else
#include <GL/freeglut.h>
#endif
#include <GL/glew.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <curand.h>
#include <helper_functions.h>
#include <helper_cuda.h>

#include "kernel.h"

static clock_t timeeeeee;
const int size = 600;
static int matrix[HEIGHT * WIDTH] = { 0 };
static int bufferMatrix[HEIGHT * WIDTH] = { 0 };
int xxCompute(int i) {
    return i % WIDTH;
}
int yyCompute(int i) {
    return i / WIDTH;
}


void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, update, 0);
}
void init() { //This will initilize a few thing
    glClearColor(0.0, 0.0, 0.0, 0.0); // Change colour on clears the screeb
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-0.5f, ((float)WIDTH - 1) - 0.5f, -0.5f, ((float)HEIGHT - 1) - 0.5f); // I can't exactly remember, but I thing this centers the matrix in the window

}
void display() { // This is the Display Loop

    printf("%d", 1000/(clock() - timeeeeee));
    cout << endl;
    timeeeeee = clock();
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(20);


    /* For These are for Vertex buffer objects if you decide to go this way
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    // Place you fuctions for Threads Here

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    */


    glBegin(GL_POINTS);
    
    doWorkCUDA(matrix, bufferMatrix, HEIGHT * WIDTH);
    for (int i = 0; i < HEIGHT * WIDTH; i++) {
        if (bufferMatrix[i] == 0) {
            glColor3f(1, 1, 1);


        }
        else if (bufferMatrix[i] == 1) {
            glColor3f(1, 0, 0);

        }
        else if (bufferMatrix[i] == 2) {
            glColor3f(0, 1, 0);

        }
        else if (bufferMatrix[i] == 3) {
            glColor3f(0, 0, 1);

        }
        else if (bufferMatrix[i] == 4) {
            glColor3f(0, 0, 0);

        }

        glVertex2i(xxCompute(i) % (WIDTH), yyCompute(i) % (HEIGHT)); // coords
        matrix[i] = bufferMatrix[i];
    }
    glEnd();
    glFlush();
}


void initMatrix(int matrix[HEIGHT * WIDTH], int size) {
    for (int i = 0; i < size; i++) {
        int random = (rand() % 5);
        if (random == 0) {
            matrix[i] = 0;

        }
        else if (random == 1) {

            matrix[i] = 1;
        }
        else if (random == 2) {

            matrix[i] = 2;
        }
        else if (random == 3) {

            matrix[i] = 3;
        }
        else if (random == 4) {

            matrix[i] = 4;
        }
    }
}
int main(int argc, char** argv)
{/*
    cout << "hello";

    const int arraySize = 5;
    const int a[arraySize] = { 1, 2, 3, 4, 5 };
    const int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(c, a, b, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }
    */
    const int arraySize1 = HEIGHT * WIDTH;

    
    initMatrix(matrix, HEIGHT * WIDTH);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_ALPHA); // Try changing these
    glutInitWindowSize(WIDTH - 1 * 50, HEIGHT - 1 * 50); // Width
    //glDisable(GL_TEXTURE_2D); sometimes recommended
    glutCreateWindow("Your Title");


    // Do Setup Work
    init();

    glutDisplayFunc(display);
    glutTimerFunc(100, update, 0); // from TA's demo

    glutMainLoop();





    return 0;
}