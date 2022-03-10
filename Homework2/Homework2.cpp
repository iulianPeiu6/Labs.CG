#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "glut.h"

#define dim 800

using namespace std;

unsigned char prevKey;

void Display1() {
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1.0);
    double ratia = 0.05;
    double t;

    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);


    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1;
        x1 = (a + b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x2, y2;
        x2 = (a - b * cos(t)) / xmax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}

void Display2() {
    double pi = 4 * atan(1.0);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}

void Display3() {
    double ratia = 0.05;
    double xmax = 50;
    double ymax = 1;
    
    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < 50; x += ratia) {
        double x1, y1;
        if (x != 0) {
            
            x1 = x;
            y1 = (abs(round(x) - x) / x);

            glVertex2f(x1 / xmax, y1 / ymax);
            continue;
        }
        x1 = 0;
        y1 = 1;

        glVertex2f(x1 / xmax, y1 / ymax);
    }
    glEnd();
}

void Display4() {
    double pi = 4 * atan(1.0);
    double a = 0.3;
    double b = 0.2;
    double ratia = 0.05;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;

    for (double t = 0; t < 100; t += ratia) {
        double x1, y1;
        x1 = 2 * (a * cos(t) + b) * cos(t);
        xmax = max(xmax, abs(x1));
        y1 = 2 * (a * cos(t) + b) * sin(t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t < 100; t += ratia) {
        double x1, y1;
        x1 = 2 * (a * cos(t) + b) * cos(t);
        y1 = 2 * (a * cos(t) + b) * sin(t);

        glVertex2f(x1 / xmax, y1 / ymax);
    }
    glEnd();
}

void Display5() {
    double pi = 4 * atan(1.0);
    double a = 0.2;
    double ratia = 0.01;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;
    double left = -pi / 2.2;
    double right = -pi / 5;
    double moveToRightBy = 0.1;
    double startX = a / (4 * cos(left + ratia) * cos(left + ratia) - 3);
    double startY = a * tan(left + ratia) / (4 * cos(left + ratia) * cos(left + ratia) - 3);
    

    for (double t = left + ratia; t < right; t += ratia) {
        if (t == pi / 6 || t == -pi / 6) {
            continue;
        }
        double x1, y1;
        x1 = a / (4 * cos(t) * cos(t) - 3);
        xmax = max(xmax, fabs(x1));
        y1 = a * tan(t) / (4 * cos(t) * cos(t) - 3);
        ymax = max(ymax, fabs(y1));
        startX = x1;
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    

    glColor3f(1, 0.1, 0.1);
    for (double t = left + ratia; t < right - 2 * ratia; t += 3 * ratia) {
        if (t == -pi / 6) {
            continue;
        }
        
        double x1, y1;

        glBegin(GL_TRIANGLES);
        glVertex2f(startX / xmax + moveToRightBy, startY / ymax);

        x1 = a / (4 * cos(t) * cos(t) - 3);
        y1 = a * tan(t) / (4 * cos(t) * cos(t) - 3);

        glVertex2f(x1 / xmax + moveToRightBy, y1 / ymax);
        x1 = a / (4 * cos(t + ratia) * cos(t + ratia) - 3);
        y1 = a * tan(t + ratia) / (4 * cos(t + ratia) * cos(t + ratia) - 3);

        glVertex2f(x1 / xmax + moveToRightBy, y1 / ymax);
        glEnd();
    }

    glColor3f(0.2, 0.15, 0.88);
    glBegin(GL_LINE_STRIP);
    glVertex2f(startX / xmax + moveToRightBy, startY / ymax);
    for (double t = left + ratia; t < right; t += ratia) {
        if (t == -pi / 6) {
            continue;
        }
        double x1, y1;
        x1 = a / (4 * cos(t) * cos(t) - 3);
        y1 = a * tan(t) / (4 * cos(t) * cos(t) - 3);
        glVertex2f(x1 / xmax + moveToRightBy, y1 / ymax);
    }
    glVertex2f(startX / xmax + moveToRightBy, startY / ymax);
    glEnd();
}

void Display6() {
    double pi = 4 * atan(1.0);
    double a = 0.1;
    double b = 0.2;
    double ratia = 0.05;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;
    double infinity = 10.0;

    for (double t = -infinity; t < infinity; t += ratia) {
        double x1, y1;
        x1 = a * t - b * sin(t);
        xmax = max(xmax, abs(x1));
        y1 = a - b * cos(t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = -infinity; t < infinity; t += ratia) {
        double x1, y1;
        x1 = a * t - b * sin(t);
        y1 = a - b * cos(t);

        glVertex2f(x1 / xmax, y1 / ymax);
    }
    glEnd();
}

void Display7() {
    double pi = 4 * atan(1.0);
    double R = 0.1;
    double r = 0.3;
    double ratia = 0.01;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;

    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R + r) * cos(r / R * t) - r * cos(t + r / R * t);
        xmax = max(xmax, abs(x1));
        y1 = (R + r) * sin(r / R * t) - r * sin(t + r / R * t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R + r) * cos(r / R * t) - r * cos(t + r / R * t);
        y1 = (R + r) * sin(r / R * t) - r * sin(t + r / R * t);
        glVertex2f(x1 / xmax, y1 / ymax);

    }
    glEnd();
}

void Display8() {
    double pi = 4 * atan(1.0);
    double R = 0.1;
    double r = 0.3;
    double ratia = 0.01;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;

    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R - r) * cos(r / R * t) - r * cos(t - r / R * t);
        xmax = max(xmax, abs(x1));
        y1 = (R - r) * sin(r / R * t) - r * sin(t - r / R * t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R - r) * cos(r / R * t) - r * cos(t - r / R * t);
        y1 = (R - r) * sin(r / R * t) - r * sin(t - r / R * t);
        
        glVertex2f(x1 / xmax, y1 / ymax);

    }
    glEnd();
}

void Display9() {
    double pi = 4 * atan(1.0);
    double a = 0.4;
    double ratia = 0.01;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;

    for (double t = - pi / 4; t <= pi / 4; t += ratia) {
        double r, x1, y1;
        r = a * sqrt(2 * cos(2 * t));
        x1 = r * cos(t);
        xmax = max(xmax, abs(x1));
        y1 = r * sin(t);
        ymax = max(ymax, abs(y1));
        r = - a * sqrt(2 * cos(2 * t));
        x1 = r * cos(t);
        xmax = max(xmax, abs(x1));
        y1 = r * sin(t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = - pi / 4; t <= pi / 4 + ratia; t += ratia) {
        double r, x1, y1;
        r = a * sqrt(2 * cos(2 * t));
        x1 = r * cos(t);
        y1 = r * sin(t);

        glVertex2f(x1 / xmax, y1 / ymax);

    }
    for (double t = - pi / 4; t <= pi / 4; t += ratia) {
        double r, x1, y1;
        r = -a * sqrt(2 * cos(2 * t));
        x1 = r * cos(t);
        y1 = r * sin(t);

        glVertex2f(x1 / xmax, y1 / ymax);

    }
    glEnd();
}

void Display10() {
    double pi = 4 * atan(1.0);
    double a = 0.02;
    double e = M_E;
    double ratia = 0.05;
    double xmax = -DBL_MAX;
    double ymax = -DBL_MAX;
    double infinity = 9.3;

    for (double t = 0; t <= infinity; t += ratia) {
        double r, x1, y1;
        r = a * pow(e, 1 + t);
        x1 = r * cos(t);
        xmax = max(xmax, abs(x1));
        y1 = r * sin(t);
        ymax = max(ymax, abs(y1));
    }
    xmax = max(xmax, ymax);
    ymax = max(xmax, ymax);

    glColor3f(1, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= infinity; t += ratia) {
        double r, x1, y1;
        r = a * pow(e, 1 + t);
        x1 = r * cos(t);
        y1 = r * sin(t);

        glVertex2f(x1 / xmax, y1 / ymax);

    }
    glEnd();
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
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
    case '9':
        Display9();
        break;
    case '0':
        Display10();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

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
