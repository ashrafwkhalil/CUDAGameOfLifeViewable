#include <vector>
#include <iostream>
#include <ctime>
#include <cstdint>
#include <stdio.h>
#ifdef _WIN32
#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif
#include <helper_gl.h>
#if defined(_APPLE_) || defined(MACOSX)
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
static int matrix[HEIGHT * WIDTH] = {0};
static int bufferMatrix[HEIGHT * WIDTH] = {0};
int xxCompute(int i)
{
    return i % WIDTH;
}
int yyCompute(int i)
{
    return i / WIDTH;
}

void update(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 30, update, 0);
}
void init()
{ // This will initilize a few things
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-0.5f, ((float)WIDTH - 1) - 0.5f, -0.5f, ((float)HEIGHT - 1) - 0.5f); // I can't exactly remember, but I thing this centers the matrix in the window
}
void display()
{ // This is the Display Loop

    printf("%d", 1000 / (clock() - timeeeeee));
    cout << endl;
    timeeeeee = clock();
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(20);

    glBegin(GL_POINTS);

    doWorkCUDA(matrix, bufferMatrix, HEIGHT * WIDTH);
    for (int i = 0; i < HEIGHT * WIDTH; i++)
    {
        if (bufferMatrix[i] == 0)
        {
            glColor3f(1, 1, 1);
        }
        else if (bufferMatrix[i] == 1)
        {
            glColor3f(1, 0, 0);
        }
        else if (bufferMatrix[i] == 2)
        {
            glColor3f(0, 1, 0);
        }
        else if (bufferMatrix[i] == 3)
        {
            glColor3f(0, 0, 1);
        }
        else if (bufferMatrix[i] == 4)
        {
            glColor3f(0, 0, 0);
        }

        glVertex2i(xxCompute(i) % (WIDTH), yyCompute(i) % (HEIGHT)); // coords
        matrix[i] = bufferMatrix[i];
    }
    glEnd();
    glFlush();
}

void initMatrix(int matrix[HEIGHT * WIDTH], int size)
{
    for (int i = 0; i < size; i++)
    {
        int random = (rand() % 5);
        if (random == 0)
        {
            matrix[i] = 0;
        }
        else if (random == 1)
        {

            matrix[i] = 1;
        }
        else if (random == 2)
        {

            matrix[i] = 2;
        }
        else if (random == 3)
        {

            matrix[i] = 3;
        }
        else if (random == 4)
        {

            matrix[i] = 4;
        }
    }
}
int main(int argc, char **argv)
{
    const int arraySize1 = HEIGHT * WIDTH;

    initMatrix(matrix, HEIGHT * WIDTH);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_ALPHA);
    glutInitWindowSize(WIDTH - 1 * 50, HEIGHT - 1 * 50);

    glutCreateWindow("Your Title");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(100, update, 0);

    glutMainLoop();

    return 0;
}