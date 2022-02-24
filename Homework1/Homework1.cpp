#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "glut.h"

unsigned char prevKey;

void Display1() {
    glColor3f(0.2, 0.15, 0.88);
    glBegin(GL_LINES);
    glVertex2i(1, 1);
    glVertex2i(-1, -1);
    glEnd();

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINES);
    glVertex2i(-1, 1);
    glVertex2i(1, -1);
    glEnd();

    glBegin(GL_LINES);
    glVertex2d(-0.5, 0);
    glVertex2d(0.5, 0);
    glEnd();
}

void Display2() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_LINES);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.9, 0.9);
    glVertex2f(0.8, 0.8);
    glVertex2f(0.7, 0.7);
    glVertex2f(0.6, 0.6);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-1.0, -1.0);
    glEnd();
}

void Display3() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_POINTS);
    glVertex2f(-0.5, -0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, 0.5);
    glEnd();
}

void Display4() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_LINE_STRIP);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, 0.9);
    glVertex2f(0.8, 0.8);
    glVertex2f(0.8, 0.7);
    glEnd();
}

void Display5() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_LINE_LOOP);
    glVertex2f(1.0, 1.0);
    glVertex2f(1.0, 0.9);
    glVertex2f(0.8, 0.8);
    glVertex2f(0.8, 0.7);
    glVertex2f(1.0, 1.0);
    glEnd();
}

void Display6() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_TRIANGLES);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.9, 0.9);
    glVertex2f(1.0, 0.9);
    glVertex2f(-1.0, -1.0);
    glVertex2f(-0.9, -0.9);
    glVertex2f(-1.0, -0.9);
    glEnd();
}

void Display7() {
    glColor3f(1, 0.1, 0.1); // red
    glBegin(GL_QUADS);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.5, 0.75);
    glVertex2f(0.6, 0.6);
    glVertex2f(1.0, 0.6);
    glEnd();
}

void drawHex(double sideDim) {
    double hexHeight = sqrt(pow(sideDim, 2) - pow(sideDim / 2, 2));

    glBegin(GL_POLYGON);
    glVertex2f(sideDim / 2, -hexHeight);
    glVertex2f(-sideDim / 2, -hexHeight);
    glVertex2f(-sideDim, -0.0);
    glVertex2f(-sideDim / 2, hexHeight);
    glVertex2f(sideDim / 2, hexHeight);
    glVertex2f(sideDim, 0.0);
    glEnd();
}

void Display8() {
    glColor3f(0.2, 0.15, 0.88); // blue
    drawHex(0.5);

    glColor3f(1, 0.1, 0.1); // red
    drawHex(0.4);

    glColor3f(1.0, 1.0, 1.0); // red
    drawHex(0.39);
}

void Init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(3);

    glPointSize(4);

    glPolygonMode(GL_FRONT, GL_FILL);
}

void Display(void) {
    printf("Call Display\n");

    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    printf("Reshape : width = %d, height = %d\n", w, h);

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    printf("You have pressed <%c>. The mouse has the position %d, %d.\n",
        key, x, y);

    prevKey = key;
    if (key == 27)
        exit(0);

    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
    printf("Call MouseFunc : you have %s the %s button in the position %d %d\n",
        (state == GLUT_DOWN) ? "pressed" : "released",
        (button == GLUT_LEFT_BUTTON) ?
        "left" :
        ((button == GLUT_RIGHT_BUTTON) ? "right" : "middle"),
        x, y);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitWindowSize(600, 600);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}
