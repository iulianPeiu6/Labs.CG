#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <complex>
#include "glut.h"

#define dim 400

unsigned char prevKey;
int nivel = 0;

class MandelbrotSet {
public:
    MandelbrotSet() {
        this->fromX = -2;
        this->fromY = -2;
        this->toX = 2;
        this->toY = 2;
        this->ratia = 0.005;
        this->maxIterations = 50;
    }

    void draw() {
        for (double x = this->fromX; x <= this->toX; x += ratia) {
            for (double y = this->fromY; y <= this->toY; y += ratia) {
                std::complex<double> c(x, y);
                if (belongsToMandelbrotSet(c)) {
                    drawPoint(c);
                }
            }
        }
    }

private:
    double fromX, fromY, toX, toY;
    double ratia;
    double maxIterations;

    bool belongsToMandelbrotSet(std::complex<double> c) {
        std::complex<double> z(0, 0);

        for (int i = 0; i < this->maxIterations; i++) {
            if (abs(z) > 2) {
                return false;
            }
            z = z * z + c;
        }

        return true;
    }

    void drawPoint(std::complex<double> c) {
        c = normalized(c);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_POINTS);
        glVertex2f(c.real(), c.imag());
        glEnd();
    }

    std::complex<double> normalized(std::complex<double> c) {
        double newReal = c.real() / 2;
        double newImag = c.imag() / 2;
        return std::complex<double>(newReal, newImag);
    }
};

class SquareThingy {
public:
    void draw(int depth) {
        drawSquare(0, 0, 1.8);
        drawRecursiveSquare(0, 0, 1.8 / 3, depth);
    }

private:
    void drawSquare(double x, double y, double len) {
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0, 0.1, 0.1);
        glVertex2d(x - len / 2, y + len / 2);
        glVertex2d(x + len / 2, y + len / 2);
        glVertex2d(x + len / 2, y - len / 2);
        glVertex2d(x - len / 2, y - len / 2);
        glVertex2d(x - len / 2, y + len / 2);
        glEnd();

    }

    void drawRecursiveSquare(double x, double y, double len, double depth) {
        drawSquare(x, y, len);

        if (depth) {
            drawRecursiveSquare(x - len, y + len, len / 3, depth - 1);
            drawRecursiveSquare(x, y + len, len / 3, depth - 1);
            drawRecursiveSquare(x + len, y + len, len / 3, depth - 1);
            drawRecursiveSquare(x + len, y, len / 3, depth - 1);
            drawRecursiveSquare(x + len, y - len, len / 3, depth - 1);
            drawRecursiveSquare(x, y - len, len / 3, depth - 1);
            drawRecursiveSquare(x - len, y - len, len / 3, depth - 1);
            drawRecursiveSquare(x - len, y, len / 3 , depth - 1);
        }
    }
};

class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}

    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};

class TreeThingy {
public:
    void draw(double lungime, int nivel)
    {
        CVector v(0.0, - 1.0);
        CPunct p(0.0, -1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        drawRecursive(lungime, nivel, 0.4, p, v, false);
    }

private:
    void drawRecursive(double lungime,
        int depth,
        double factordiviziune,
        CPunct p,
        CVector v,
        bool branchType)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (depth == 0)
        {
        }
        else
        {
            if (!branchType) {
                v.rotatie(-45);
            } 
            else {
                v.rotatie(-90);
            }
            
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            drawRecursive(lungime * factordiviziune, depth - 1, factordiviziune, p1, v, branchType);

            if (!branchType) {
                v.rotatie(90);
            }
            else {
                v.rotatie(120);
            }

            v.deseneaza(p, lungime);
            p2 = v.getDest(p, lungime);

            v.rotatie(-45);
            v.deseneaza(p2, lungime);
            p1 = v.getDest(p2, lungime);
            drawRecursive(lungime * factordiviziune, depth - 1, factordiviziune, p1, v, !branchType);

            v.rotatie(60);
            v.deseneaza(p2, lungime);
            p1 = v.getDest(p2, lungime);
            drawRecursive(lungime * factordiviziune, depth - 1, factordiviziune, p1, v, branchType);
        }
    }
};

class TriangleThingy {
public:
    void draw(int depth) {
        drawRecursiveTriangle(0.1, 0.9, 0.8, 0, depth);
    }
private:
    void drawTriangleSide(double x, double y, double len, int side) {
        assert(side <= 2);
        assert(side >= 0);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0, 0.1, 0.1);
        if (side == 0) {
            glVertex2d(normalized(x), normalized(y));
            glVertex2d(normalized(x), normalized(y - len));
        }
        else if (side == 1) {
            glVertex2d(normalized(x), normalized(y));
            glVertex2d(normalized(x + len / 2 * sqrt(3)), normalized(y - len / 2));
        }
        else if (side == 2) {
            glVertex2d(normalized(x + len / 2 * sqrt(3)), normalized(y - len / 2));
            glVertex2d(normalized(x), normalized(y - len));
        }
        glEnd();
    }

    void drawRecursiveTriangle(double x, double y, double len, int rotationType, int depth) {
        if (depth == 0) {
            drawTriangleSide(x, y, len, rotationType);
        }
        else {
            drawRecursiveTriangle(x + len / 4 * sqrt(3), y - len / 4, len / 2, rotationType, depth - 1);
            rotationType = (rotationType + depth + depth / 3) % 3;
            drawRecursiveTriangle(x, y, len / 2, rotationType, depth - 1);
            rotationType = (rotationType + depth + depth / 3) % 3;
            drawRecursiveTriangle(x, y - len / 2, len / 2, rotationType, depth - 1);
        }
    }
private:
    double normalized(double x) {
        // x = [0;1]  -> x' = [-1;1] 
        return x * 2 - 1;
    }
};

void Display1() {
    MandelbrotSet mandelbrotSet = MandelbrotSet();
    mandelbrotSet.draw();
}

void Display2(int depth) {
    SquareThingy squareThingy = SquareThingy();
    squareThingy.draw(depth);
}

void Display3(int depth) {
    glPushMatrix();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, 2.5, 0.0);
    TreeThingy treeThingy = TreeThingy();
    treeThingy.draw(1, depth);
    glPopMatrix();
}

void Display4(int depth) {
    TriangleThingy triangleThingy = TriangleThingy();
    triangleThingy.draw(depth);
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    switch (prevKey) {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        nivel++;
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2(nivel);
        nivel++;
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3(nivel);
        nivel++;
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
        Display4(nivel);
        nivel++;
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
