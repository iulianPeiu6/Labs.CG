#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <complex>
#include <vector>
#include <tuple>
#include <fstream>
#include <algorithm>
#include "glut.h"

#define DIM 800
#define PI 3.14159265358979323846

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
		printf("Drawing point (%d, %d)\n", i, j);
		if (i < 0 || j < 0 || i > this->width || j > this->height) {
			return;
		}
		float angle;
		double radius = 1.0 / this->width / 3;

		double x1 = 1.0 / this->width * i;
		double y1 = 1.0 / this->height * j;
		glColor3f(0.0, 0.0, 0.0);

		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(Normalized(x1), Normalized(y1));

		for (angle = 0.0; angle < 2 * PI; angle += 0.01) {
			double x2 = x1 + sin(angle) * radius;
			double y2 = y1 + cos(angle) * radius;
			glVertex2f(Normalized(x2), Normalized(y2));
		}

		glEnd();
		glFlush();
	}

	void DrawMatrixPoints(int i, int j, int width) {
		for (int x = i - width + 1; x <= i + width - 1; x++) {
			for (int y = j - width + 1; y <= j + width - 1; y++) {
				DrawPoint(x, y);
			}
		}
	}

	void DrawMatrixPoints(int fromX, int toX, int fromY, int toY) {
		for (int x = fromX; x <= toX; x++) {
			for (int y = fromY; y <= toY; y++) {
				DrawPoint(x, y);
			}
		}
	}

	void DrawCircle(int centerI, int centerJ, int radius, int width) {
		DrawRawCircle(centerI, centerJ, radius);

		int x = centerI, y = centerJ + radius;
		int d = 1 - radius;
		int dE = 3, dSE = -2 * radius + 5;

		do {
			/*DrawMatrixPoints(x, y, width);
			DrawMatrixPoints(x - 2 * abs(centerI - x), y - 2 * abs(centerJ - y), width);
			DrawMatrixPoints(x - 2 * abs(centerI - x), y, width);
			DrawMatrixPoints(x, y - 2 * abs(centerJ - y), width);*/
			DrawMatrixPoints(y, x, width);
			/*DrawMatrixPoints(y - 2 * abs(centerJ - y), x - 2 * abs(centerI - x), width);
			DrawMatrixPoints(y, x - 2 * abs(centerI - x), width);
			DrawMatrixPoints(y - 2 * abs(centerJ - y), x, width);*/
			if (d < 0) {
				d += dE;
				dE += 2;
				dSE += 2;
			}
			else {
				d += dSE;
				dE += 2;
				dSE += 4;
				y--;
			}
			x++;
		} while (y > x);
	}

	void DrawFilledElipse(int x0, int y0, int a, int b) {
		DrawRawElipse(x0, y0, a, b);

		int xi = 0, x = 0, y = b;
		double fxpyp = 0.0;
		double deltaE, deltaSE, deltaS;

		// 1st region
		do {
			/*DrawMatrixPoints(xi + x0, x + x0, y + y0, y + y0);
			DrawMatrixPoints(xi + x0, x + x0, y0 - y, y0 - y);*/
			DrawMatrixPoints(x0 - x, x0 - xi, y0 - y, y0 - y);
			/*DrawMatrixPoints(x0 - x, x0 - xi, y + y0, y + y0);*/

			deltaE = b * b * (2 * x + 1);
			deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
			if (fxpyp + deltaE <= 0.0) {
				// E is inside
				fxpyp += deltaE;
				x++;
				/*DrawMatrixPoints(xi + x0, x + x0, y + y0, y + y0);
				DrawMatrixPoints(xi + x0, x + x0, y0 - y, y0 - y);*/
				DrawMatrixPoints(x0 - x, x0 - xi, y0 - y, y0 - y);
				/*DrawMatrixPoints(x0 - x, x0 - xi, y + y0, y + y0);*/
			}
			else if (fxpyp + deltaSE <= 0.0) {
				// SE is inside
				fxpyp += deltaSE;
				x++; y--;
				/*DrawMatrixPoints(xi + x0, x + x0, y + y0, y + y0);
				DrawMatrixPoints(xi + x0, x + x0, y0 - y, y0 - y);*/
				DrawMatrixPoints(x0 - x, x0 - xi, y0 - y, y0 - y);
				/*DrawMatrixPoints(x0 - x, x0 - xi, y + y0, y + y0);*/
			}
		} while (a * a * (y - 0.5) > b * b * (x + 1));

		// 2nd region
		do {
			/*DrawMatrixPoints(xi + x0, x + x0, y + y0, y + y0);
			DrawMatrixPoints(xi + x0, x + x0, y0 - y, y0 - y);*/
			DrawMatrixPoints(x0 - x, x0 - xi, y0 - y, y0 - y);
			/*DrawMatrixPoints(x0 - x, x0 - xi, y + y0, y + y0);*/

			deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
			deltaS = a * a * (-2 * y + 1);
			if (fxpyp + deltaSE <= 0.0) {
				// SE is inside
				fxpyp += deltaSE;
				x++; y--;
			}
			else {
				// S is inside
				fxpyp += deltaS;
				y--;
			}
		} while (y >= 0);
	}

	void DrawFilledPolygon(int dim, std::vector<std::pair<int, int>> vertices) {
		DrawRawPolygon(dim, vertices);
		
		std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> edges;

		struct Intersection {
			int ymax;
			double xmin, ratia;
		};
		

		// Set edges 
		for (int i = 1; i < dim; i++)
		{
			edges.push_back({ vertices[i - 1], vertices[i] });
		}
		edges.push_back({ vertices.back(), vertices.front() });

		// Initialise et
		std::vector<std::vector <Intersection>> et(this->height);

		for (auto edge : edges) {
			if (edge.first.second != edge.second.second) {
				int ym = std::min(edge.first.second, edge.second.second);
				int yM = std::max(edge.first.second, edge.second.second);
				int xm = (ym == edge.first.second) ? edge.first.first : edge.second.first;
				int xM = (yM == edge.first.second) ? edge.first.first : edge.second.first;
				et[ym].push_back({ yM, 1.0 * xm, 1.0 * (xm - xM) / (ym - yM) });
			}
		}

		for (auto& it : et) {
			sort(it.begin(), it.end(), [](Intersection A, Intersection B) {return A.xmin < B.xmin; });
		}

		// SSM Computation
		std::vector<std::vector <Intersection>> finalEt(et.size());
		std::vector<Intersection> activeSSM;

		for (int y = 0; y < this->height; y++) {
			activeSSM.insert(activeSSM.end(), et[y].begin(), et[y].end());
			int pos = 0;
			for (int i = 0; i < activeSSM.size(); i++) {
				if (activeSSM[i].ymax == y) {
					continue;
				}
				activeSSM[pos++] = activeSSM[i];
			}

			while (activeSSM.size() > pos) {
				activeSSM.pop_back();
			}

			sort(activeSSM.begin(), activeSSM.end(), [](Intersection A, Intersection B) {return A.xmin < B.xmin; });

			finalEt[y] = activeSSM;

			for (auto& it : activeSSM) {
				it.xmin += it.ratia;
			}
		}

		// Drawing
		const double EPS = 1e-6;
		for (int y = 0; y < this -> height; y++) {
			for (int i = 1; i < finalEt[y].size(); i += 2) {
				for (int x = finalEt[y][i - 1].xmin; x < finalEt[y][i].xmin; x++) {
					if (x < finalEt[y][i - 1].xmin - EPS) {
						continue;
					}
					DrawPoint(x, y);
				}
				if (abs(finalEt[y][i - 1].xmin - finalEt[y][i].xmin) < EPS) {
					DrawPoint(finalEt[y][i].xmin, y);
				}
			}
		}
	}

private:
	double xmin, ymin, xmax, ymax;
	double width, height;
	std::vector <std::pair<std::pair<int, int>, std::pair<int, int>>> polygon;

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

	void DrawRawLine(int fromI, int fromJ, int toI, int toJ) {
		double x1 = 1.0 / this->width * fromI;
		double y1 = 1.0 / this->height * fromJ;
		double x2 = 1.0 / this->width * toI;
		double y2 = 1.0 / this->height * toJ;

		glBegin(GL_LINES);
		glColor3f(1.0, 0.1, 0.1);
		glVertex2d(Normalized(x1), Normalized(y1));
		glVertex2d(Normalized(x2), Normalized(y2));
		glEnd();
	}

	void DrawRawCircle(int centerI, int centerJ, int radius) {
		int i;
		int lineAmount = 100;
		double x = 1.0 / this->width * centerI;
		double y = 1.0 / this->height * centerJ;

		GLfloat r = 1.0 / this->width * radius;

		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.1, 0.1);
		for (i = 0; i <= lineAmount; i++) {
			glVertex2f(
				Normalized(x + (r * cos(i * 2.0f * PI / lineAmount))),
				Normalized(y + (r * sin(i * 2.0f * PI / lineAmount)))
			);
		}
		glEnd();
	}

	void DrawRawElipse(int x0, int y0, int a, int b)
	{
		double cx = 1.0 / this->width * x0;
		double cy = 1.0 / this->height * y0;
		double rx = 1.0 / this->width * a;
		double ry = 1.0 / this->width * b;
		int num_segments = 100;
		double theta = 2 * 3.1415926 / float(num_segments);
		double c = cosf(theta);
		double s = sinf(theta);
		double t;

		double x = 1;
		double y = 0;

		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.1, 0.1);
		for (int ii = 0; ii < num_segments; ii++)
		{
			glVertex2f(Normalized(x * rx + cx), Normalized(y * ry + cy));

			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		}
		glEnd();
	}

	void DrawRawPolygon(int dim, std::vector<std::pair<int, int>> vertices) {
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.1, 0.1);
		for (int i = 0; i < dim; i++) {
			double x = 1.0 / this->width * vertices[i].first;
			double y = 1.0 / this->width * vertices[i].second;
			glVertex2f(Normalized(x), Normalized(y));
		}
		glEnd();
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

	CartesianGrid grid = CartesianGrid(50, 50);

	//grid.DrawCircle(25, 25, 20, 2);

	//grid.DrawFilledElipse(21, 30, 20, 10);

	std::ifstream f("polygon.txt");

	int dim;
	std::vector<std::pair<int, int>> vertices;
	f >> dim;
	for (int i = 0; i < dim; i++) {
		int x, y;
		f >> x >> y;
		vertices.push_back(std::pair<int, int>(x, y));
	}
	f.close();

	grid.DrawFilledPolygon(dim, vertices);
	

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
