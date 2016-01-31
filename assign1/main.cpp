#ifdef WIN32
	#include <windows.h>
#endif

#define GLFW_DLL
#include<GLFW/glfw3.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include "misc.h"
#include <vector>
#include <stdio.h>

#define RAD_STEP	M_PI / 180
#define STEP        0.01

bool refresh;
bool animate;

float R = 1;           // outer circle radius
float r = 0.333333;    // inner circle radius
float speed = 2;
float scale = 1;

int cycles = 1;
int rotation = 0;

float timer;
float FRAMETIME = 1.0f / 60;

void drawPoints(std::vector<Point3D*> points)
{
    for (std::vector<Point3D*>::iterator it = points.begin(); it != points.end(); ++it)
    {
        Point3D *pt = (*it);

        glVertex3f(pt->x, pt->y, pt->z);
    }
}

void render(std::vector<Point3D*> p_h, std::vector<Point3D*> p_r, std::vector<Point3D*> p_l, std::vector<Point3D*> p_R)
{
    glEnable (GL_DEPTH_TEST);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Functions for changing transformation and projection matrices
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(1, 1, 1.0f);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho (-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glBegin (GL_LINE_STRIP); 
    glColor3f (1.0f, 0.0f, 0.0f);
    drawPoints(p_h);
    glEnd ();

    glBegin (GL_LINE_STRIP);
    glColor3f (1.0f, 1.0f, 1.0f);
    drawPoints(p_r);
    glEnd ();

    glBegin (GL_LINE_STRIP); 
    glColor3f (1.0f, 1.0f, 1.0f);
    drawPoints(p_l);
    glEnd ();

    glBegin (GL_LINE_STRIP);
    glColor3f (0.0f, 0.0f, 1.0f);
    drawPoints(p_R);
    glEnd ();
}

Point3D *getHCPoint(float r, float R, double theta)
{
    double x = (R - r)*cos(theta) + r*cos(((R - r) / r)*theta);
    double y = (R - r)*sin(theta) - r*sin(((R - r) / r)*theta);

    Point3D *result = new Point3D(x, y, 0);

    return result;
}

void circlePoints(std::vector<Point3D*> *points, float radius, Point3D pos)
{
    for (double theta = 0; theta < 2 * M_PI; theta += RAD_STEP)
    {
        double x = radius * cos(theta) + pos.x;
        double y = radius * sin(theta) + pos.y;

        points->push_back(new Point3D(x, y, 0));
    }
}

void makeCycloid(std::vector<Point3D*> *points, int n, float r, float R, float rot, float s)
{
    float theta = 0;

    for (int cycle = 0; cycle < n; cycle++)
    {
        for (float t = 0; t < 2 * M_PI; t += RAD_STEP)
        {
            points->push_back(getHCPoint(r, R, theta));
            theta += RAD_STEP;
        }
    }
}

Point3D *innerPath(float r, float R, double theta)
{
    double x = (R - r)*cos(theta);
    double y = (R - r)*sin(theta);

    Point3D *result = new Point3D(x, y, 0);

    return result;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS | action == GLFW_REPEAT)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		case GLFW_KEY_LEFT:
			if (r > STEP)
				r -= STEP;
			printf("Inner Radius: %.2f\n", r);
			break;
		case GLFW_KEY_RIGHT:
			if ((r + STEP) < R)
				r += STEP;
			printf("Inner Radius: %.2f\n", r);
			break;
		case GLFW_KEY_UP:
			if (R > STEP)
				R -= STEP;
			printf("Outer Radius: %.2f\n", R);
			break;
		case GLFW_KEY_DOWN:
			R += STEP;
			printf("Outer Radius: %.2f\n", R);
			break;
		case GLFW_KEY_MINUS:
			cycles--;
			printf("Cycles: %d\n", cycles);
			return;
		case GLFW_KEY_EQUAL:
			cycles++;
			printf("Cycles: %d\n", cycles);
			return;
		case GLFW_KEY_A:
			rotation -= STEP;
			printf("Rotation: %.2f\n", rotation);
			return;
		case GLFW_KEY_S:
			rotation += STEP;
			printf("Rotation: %.2f\n", rotation);
			return;
		case GLFW_KEY_SPACE:
			animate = true;
			printf("Rotation: %.2f\n", rotation);
			break;
		default:
			return;
		}
		refresh = true;
	}
}

void clearVector(std::vector<Point3D*> *list)
{
	while (!list->empty())
	{
		std::vector<Point3D*>::iterator it = list->end();
		it--;
		delete((*it));
		list->erase(it);
	}
}

void drawHypocycloid(std::vector<Point3D*> *points_h, 
					std::vector<Point3D*> *points_R, 
					std::vector<Point3D*> *points_r, 
					std::vector<Point3D*> *points_l)
{
	clearVector(points_h);
	clearVector(points_R);
	clearVector(points_r);
	clearVector(points_l);

	circlePoints(points_R, R, Point3D(0, 0, 0));
	for (float theta = 0; theta < M_PI * 2 * cycles; theta += RAD_STEP)
	{
		points_h->push_back(getHCPoint(r, R, theta));
	}
}

void updateAnim(float *p_theta, 
				std::vector<Point3D*> *points_h, 
				std::vector<Point3D*> *points_R, 
				std::vector<Point3D*> *points_r, 
				std::vector<Point3D*> *points_l)
{
	float &theta = *p_theta;

	timer = FRAMETIME;
	if (refresh)
	{
		theta = 0;
		clearVector(points_h);
		refresh = false;
	}

	theta += FRAMETIME * speed;

	if (theta < M_PI * 2 * cycles)
	{
		clearVector(points_R);
		clearVector(points_r);
		clearVector(points_l);

		circlePoints(points_R, R, Point3D(0, 0, 0));

		Point3D *innerPos = innerPath(r, R, theta);
		circlePoints(points_r, r, *innerPos);

		points_l->push_back(innerPos);                   // center point
		points_l->push_back(getHCPoint(r, R, theta));
		points_h->push_back(getHCPoint(r, R, theta));
	}
	else
	{
		animate = false;			// end animation
	}
}

int main () {

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

    glfwSetKeyCallback(window, key_callback);

    std::vector<Point3D*> points_h;		// hypocycloid
    std::vector<Point3D*> points_R;		// large circle
    std::vector<Point3D*> points_r;		// small circle
    std::vector<Point3D*> points_l;		// line of small circle

	float theta = 0;
	double currTime = 0;
	double prevTime = 0;

    while (!glfwWindowShouldClose (window))
    {
        // to animate. keep adding the hypocycloid points.
        // generate the outer circle points once.
        // refresh the innercircle.

		currTime = glfwGetTime();
		double elapsedTime = currTime - prevTime;
		timer -= elapsedTime;
		prevTime = currTime;

		if (timer <= 0)
		{
			if (!animate)
			{
				if (refresh)
				{
					drawHypocycloid(&points_h, &points_R, &points_r, &points_l);
				}
				refresh = false;
			}
			else
			{
				updateAnim(&theta, &points_h, &points_R, &points_r, &points_l);
			}
			render(points_h, points_R, points_r, points_l);
			glfwSwapBuffers(window);
		}

		glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}