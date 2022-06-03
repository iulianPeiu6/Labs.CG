#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "glut.h"

// the size of the window measured in pixels
#define dim 300
#define EPS 0.00001
using namespace std;
unsigned char prevKey;

enum EObiect { cubw, cubs, sferaw, sferas, plane } ob = cubw;

vector<vector<vector<float>>> figure;

void DisplayAxe() {
    int X, Y, Z;
    X = Y = 200;
    Z = 200;

    glLineWidth(2);

    // Ox axis - green
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(X, 0, 0);
    glEnd();

    // Oy axis - blue
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, Y, 0);
    glEnd();

    // Oz axis - red
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, Z);
    glEnd();

    glLineWidth(1);
}

// wireframe cube
void Display1() {
    glColor3f(1, 0, 0);
    glutWireCube(1);
}

// solid cube
void Display2() {
    glColor3f(1, 0, 0);
    glutSolidCube(1);
}

// wireframe sphere
void Display3() {
    glColor3f(0, 0, 1);
    glutWireSphere(1, 10, 10);
}

// solid sphere
void Display4() {
    glColor3f(0, 0, 1);
    glutSolidSphere(1, 10, 10);
}

void paintPlane(vector<vector<float>> triangle) {
    int X, Y, Z;
    int A, B, C;
    X = 0; Y = 1; Z = 2;
    A = 0; B = 1; C = 2;

    glBegin(GL_TRIANGLES);
    glColor3f(0, 0, 0);
    glVertex3f(triangle[A][X], triangle[A][Y], triangle[A][Z]);
    //glColor3f(1, 0, 0);
    glVertex3f(triangle[B][X], triangle[B][Y], triangle[B][Z]);
    //glColor3f(0, 0, 1);
    glVertex3f(triangle[C][X], triangle[C][Y], triangle[C][Z]);
    glEnd();
}

void translatePlane() {
    int X, Y, Z;
    int A, B, C;
    X = 0; Y = 1; Z = 2;
    A = 0; B = 1; C = 2;

    vector<vector<float>> plane = figure[0];
    float tx = -plane[A][X];
    float ty = -plane[A][Y];
    float tz = -plane[A][Z];

    for (int point = 0; point < 3; point++) {
        plane[point][X] += tx;
        plane[point][Y] += ty;
        plane[point][Z] += tz;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (abs(plane[i][j]) < EPS) {
                plane[i][j] = 0;
            }
        }
    figure[0] = plane;
    paintPlane(plane);

}

void rotateTriangleX() {
    int X, Y, Z;
    int A, B, C;
    X = 0; Y = 1; Z = 2;
    A = 0; B = 1; C = 2;
    vector<vector<float>> plane = figure[0];

    float angle = atan(plane[B][Y] / plane[B][Z]);
    double sin_angle = sin(angle);
    double cos_angle = cos(angle);

    for (int point = 0; point < 3; point++) {
        float y = plane[point][Y];
        float z = plane[point][Z];
        plane[point][Y] = y * cos_angle - z * sin_angle;
        plane[point][Z] = z * cos_angle + y * sin_angle;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (abs(plane[i][j]) < EPS) {
                plane[i][j] = 0;
            }
        }

    figure[0] = plane;
    paintPlane(plane);
}

void rotatePlaneY() {
    int X, Y, Z;
    int A, B, C;
    X = 0; Y = 1; Z = 2;
    A = 0; B = 1; C = 2;
    vector<vector<float>> plane = figure[0];

    float angle = atan(plane[B][X] / plane[B][Z]);
    double sin_angle = sin(angle);
    double cos_angle = cos(angle);

    for (int point = 0; point < 3; point++) {
        float z = plane[point][Z];
        float x = plane[point][X];
        plane[point][Z] = z * cos_angle - x * sin_angle;
        plane[point][X] = x * cos_angle + z * sin_angle;
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (abs(plane[i][j]) < EPS) {
                plane[i][j] = 0;
            }
        }

    figure[0] = plane;
    paintPlane(plane);
}

void rotateTriangleZ() {
    int X, Y, Z;
    int A, B, C;
    X = 0; Y = 1; Z = 2;
    A = 0; B = 1; C = 2;
    vector<vector<float>> plane = figure[0];
    float angle = atan(plane[C][X] / plane[C][Y]);
    double sin_angle = sin(angle);
    double cos_angle = cos(angle);

    for (int point = 0; point < 3; point++) {
        float y = plane[point][Y];
        float x = plane[point][X];
        plane[point][X] = x * cos_angle - y * sin_angle;
        plane[point][Y] = y * cos_angle + x * sin_angle;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            if (abs(plane[i][j]) < EPS) {
                plane[i][j] = 0;
            }
        }
    figure[0] = plane;
    cout << plane[A][X] << " " << plane[A][Y] << " " << plane[A][Z] << endl;
    cout << plane[B][X] << " " << plane[B][Y] << " " << plane[B][Z] << endl;
    cout << plane[C][X] << " " << plane[C][Y] << " " << plane[C][Z] << endl;
    paintPlane(plane);
}


void Display5() {
    vector<vector<float>> plane;
    vector<float> a = { 10.0, 2.0, 10.0 };
    vector<float> b = { 10.0, 5.0, 20.0 };
    vector<float> c = { 2.0, 5.0, 2.0 };
    plane.push_back(a);
    plane.push_back(b);
    plane.push_back(c);

    figure.clear();
    figure.push_back(plane);
    paintPlane(plane);
}

void DisplayObiect()
{
    switch (ob)
    {
    case cubw:
        Display1();
        break;
    case cubs:
        Display2();
        break;
    case sferaw:
        Display3();
        break;
    case sferas:
        Display4();
        break;
    case plane:
        Display5();
        break;
    default:
        break;
    }
}

// the rotation with 10 degrees about the Ox axis
void DisplayX() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(10, 1, 0, 0);
}

// the rotation with 10 degrees about the Oy axis
void DisplayY() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(10, 0, 1, 0);
}

// the rotation with 10 degrees about the Oz axis
void DisplayZ() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(10, 0, 0, 1);
}

// translation by 0.2, 0.2, 0.2
void DisplayT() {
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.2, 0.2, 0.2);
}

// scaling by 1.2, 1.2, 1.2
void DisplayS() {
    glMatrixMode(GL_MODELVIEW);
    glScalef(1.2, 1.2, 1.2);
}

void Init(void) {
    glClearColor(1, 1, 1, 1);
    glLineWidth(2);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, 30, -30);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(20, 1, 0, 0);
    glRotated(-20, 0, 1, 0);
}

void Display(void) {
    switch (prevKey)
    {
    case 'a':
        DisplayAxe();
        break;
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        break;
    case '1':
        Display1();
        ob = cubw;
        break;
    case '2':
        Display2();
        ob = cubs;
        break;
    case '3':
        Display3();
        ob = sferaw;
        break;
    case '4':
        Display4();
        ob = sferas;
        break;
    case '5':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayAxe();
        Display5();
        ob = plane;
        break;
    case 'x':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayX();
        DisplayAxe();
        DisplayObiect();
        break;
    case 'y':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayY();
        DisplayAxe();
        DisplayObiect();
        break;
    case 'z':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayZ();
        DisplayAxe();
        DisplayObiect();
        break;
    case 't':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayT();
        DisplayAxe();
        DisplayObiect();
        break;
    case 's':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayS();
        DisplayAxe();
        DisplayObiect();
        break;
    case 'm':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayAxe();
        translatePlane();
        break;
    case 'n':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayAxe();
        rotateTriangleX();
        break;
    case 'b':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayAxe();
        rotateTriangleZ();
        break;
    case 'v':
        glClear(GL_COLOR_BUFFER_BIT);
        DisplayAxe();
        rotatePlaneY();
        break;
    default:
        break;
    }
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}
