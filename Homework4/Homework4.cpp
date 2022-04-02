#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <complex>
#include "glut.h"

#define DIM 400

struct Coordonate {
    double x;
    double y;

    Coordonate(double x, double y) {
        this->x = x;
        this->y = y;
    }
};

class CartesianGrid {
public:
    CartesianGrid(int width, int height) {
        this->xmin = 0.1;
        this->ymin = 0.1;
        this->xmax = 0.9;
        this->ymax = 0.9;
        this->width = width;
        this->height = height;
        InitGrid();
    }

    void DrawPoint(int i, int j) {
        if (i < 0 || j < 0 || i > this->width || j > this->height)
        {
            return;
        }
        float angle;
        double radius = 1.0 / this->width / 3;

        double x1 = 1.0 / this->width * i;
        double y1 = 1.0 / this->height * j;
        glColor3f(0.0, 0.0, 0.0);

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(Normalized(x1), Normalized(y1));

        for (angle = 0.0; angle < 2 * 3.14159; angle += 0.2)
        {
            double x2 = x1 + sin(angle) * radius;
            double y2 = y1 + cos(angle) * radius;
            glVertex2f(Normalized(x2), Normalized(y2));
        }

        glEnd();
        glFlush();
    }

    void DrawMatrixPoints(int fromX, int toX, int fromY, int toY) {
        for (int x = fromX; x <= toX; x++) {
            for (int y = fromY; y <= toY; y++) {
                DrawPoint(x, y);
            }
        }
    }

    void DrawLine(int fromI, int fromJ, int toI, int toJ, int width = 1) {
        float slope = 1.0 * (toJ - fromJ) / (toI - fromI);

        width--;
        if (toI < fromI) {
            std::swap(fromI, toI);
            std::swap(fromJ, toJ);
        }


        int dx = (toI - fromI);
        int dy = (toJ - fromJ);
        int direction = toJ > fromJ ? -1 : 1;
        int d = 2 * dy + direction * dx;
        int dLine = 2 * dy;
        int dDiag = 2 * (dy + direction * dx);
        int i = fromI, j = fromJ;
        DrawMatrixPoints(i, i, j - width, j + width);
        while (i < toI) {
            if (d * direction > 0) {
                d += dLine;
            }
            else {
                d += dDiag;
                j += -direction;
            }
            i++;
            DrawMatrixPoints(i, i, j - width, j + width);
        }
        
    }

private:
    double xmin, ymin, xmax, ymax;
    double width, height;

    void InitGrid() {
        DrawGrid();
    }

    void DrawGrid() {
        for (int i = 0; i <= this->width; i++) {
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 0.0);
            double x = 1.0 / this->width * i;
            glVertex2d(Normalized(x), Normalized(0));
            glVertex2d(Normalized(x), Normalized(1));
            glEnd();
        }

        for (int j = 0; j <= this->height; j++) {
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 0.0);
            double y = 1.0 / this->height * j;
            glVertex2d(Normalized(0), Normalized(y));
            glVertex2d(Normalized(1), Normalized(y));
            glEnd();
        }
    }

    double Normalized(double x) {
        // [0, 1] -> [-1, 1] -> [-0.9, 0.9]
        return (x * 2 - 1) * 0.9;
    }
};

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {

    glClear(GL_COLOR_BUFFER_BIT);

    CartesianGrid grid = CartesianGrid(15, 15);

    
    grid.DrawLine(0, 0, 15, 7);
    grid.DrawLine(0, 15, 15, 10, 2);

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(DIM, DIM);

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
