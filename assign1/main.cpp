#include <windows.h>

#define GLFW_DLL
#include<GLFW/glfw3.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "misc.h"
#include <vector>

void drawPoints(std::vector<Point3D*> points)
{
	for (std::vector<Point3D*>::iterator it = points.begin(); it != points.end(); ++it)
	{
		Point3D *pt = (*it);

		glVertex3f(pt->x, pt->y, pt->z);
	}
}

void render(std::vector<Point3D*> points)
{
	glEnable (GL_DEPTH_TEST);
	//glEnable (GL_LIGHTING);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Functions for changing transformation and projection matrices
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, 0.0f);
	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glScalef(1, 1, 1.0f);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho (-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//gluPerspective (fov, aspect ratio, near plane, far plane)
	//glFrustum


	//We draw a square on the screen, which gets transformed by the model view matrix
	glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
	//glNormal3f
	glColor3f (1.0f, 0.0f, 0.0f);
	glVertex3f (-0.5f, 0.5f, 0.0f);
	glColor3f (0.0f, 1.0f, 0.0f);
	glVertex3f (0.5f, 0.5f, 0.0f);
	glColor3f (0.0f, 0.0f, 1.0f);
	glVertex3f (0.5f, -0.5f, 0.0f);
	glColor3f (1.0f, 1.0f, 1.0f);
	glVertex3f (-0.5f, -0.5f, 0.0f);
	
	drawPoints(points);
	
	glEnd ();
}

Point3D *GetPoint(float r, float R, double theta)
{
	double x = (R - r)*cos(theta) + r*cos(((R - r) / r)*theta);
	double y = (R - r)*sin(theta) - r*sin(((R - r) / r)*theta); 
	
	Point3D *result = new Point3D(x, y, 0);	

	return result;
}


void makeCycloid(std::vector<Point3D*> *points, int n, float r, float R, float rot, float s)
{
	float step = M_1_PI / 10;	// pi / 100
	float theta = 0;

	points->clear();

	for (int cycle = 0; cycle < n; cycle++)
	{
		for (float t = 0; t < 2 * M_PI; t += step)
		{
			points->push_back(GetPoint(r, R, theta));
			theta += step;
		}
	}
}

void testo(std::vector<Point3D*> *points)
{
	points->push_back(new Point3D(0, 0, 0));
}

void runTest()
{
	std::vector<Point3D*> p;
	testo(&p);
	float t1 = sin(M_PI_2);
	float t2 = sin(M_PI_4);
	/*
	Point2D *actual = GetPoint(3, 20, 0);
	Point2D *actual2 = GetPoint(3, 20, M_PI_2);
	Point2D *actual3 = GetPoint(3, 20, M_PI / 8);
	Point2D *actual4 = GetPoint(3, 20, M_PI);
	if (actual->x == 0)
	{

	}
	*/
}

int main () {
	runTest();

	if (!glfwInit()) {
		std::cout << "Could not initialize.\n";
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow (640, 640, "My Window", NULL, NULL);
	if (!window) {
		std::cout << "Could not create window.\n";
		glfwTerminate();
		return 1;
	}


	glfwMakeContextCurrent (window);

	std::vector<Point3D*> points;

	while (!glfwWindowShouldClose (window)) 
	{
		makeCycloid(&points, 5, 0.123, 1, 0, 1);
		render (points);
		glfwSwapBuffers (window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

