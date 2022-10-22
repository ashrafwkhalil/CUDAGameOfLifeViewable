#include "G:\vs\repos\A3426\A3426\Dependencies\glew\include\GL\glew.h" // must be first
#include "G:\vs\repos\A3426\A3426\Dependencies\freeglut\include\GL\freeglut.h"
#include <iostream>
#include <string>
#include <thread>
using namespace std;
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>


void display() { // This is the Display Loop


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