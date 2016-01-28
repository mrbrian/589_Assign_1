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

#define STEP	M_PI / 180

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
//	glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
    glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
    //glNormal3f
    glColor3f (1.0f, 0.0f, 0.0f);
    drawPoints(p_h);
    glEnd ();

    glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
    glColor3f (1.0f, 1.0f, 1.0f);
    drawPoints(p_r);
    glEnd ();

    glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
    glColor3f (1.0f, 1.0f, 1.0f);
    drawPoints(p_l);
    glEnd ();

    glBegin (GL_LINE_STRIP); //GL_POINTS, GL_LINES, GL_TRIANGLES, GL_QUADS
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
    for (double theta = 0; theta < 2 * M_PI; theta += STEP)
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
        for (float t = 0; t < 2 * M_PI; t += STEP)
        {
            points->push_back(getHCPoint(r, R, theta));
            theta += STEP;
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

double getInnerTheta(float r, float R, double theta)
{
    double distTravelled = theta * R;
    float innerCircum = 2 * r * M_PI;
    float innerNumRotations = distTravelled / innerCircum;

    return innerNumRotations * M_2_PI;
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

    std::vector<Point3D*> points_h;		// hypocycloid
    std::vector<Point3D*> points_R;		// bigger circle
    std::vector<Point3D*> points_r;		// smaller circle
    std::vector<Point3D*> points_l;		// line

    double R = 1;
    double r = 0.5;

    circlePoints(&points_R, R, Point3D(0, 0, 0));

        double prevTime = 0;

    while (!glfwWindowShouldClose (window))
    {
        // to animate. keep adding the hypocycloid points.
        // generate the outer circle points once.
        // refresh the innercircle.

        double currTime = glfwGetTime();
            float elapsedTime = (float)(currTime - prevTime) / 1000.f;
            prevTime = currTime;

        double theta = currTime;

        points_r.clear();
        Point3D *innerPos = innerPath(r, R, theta);
        circlePoints(&points_r, r, *innerPos);

        points_l.clear();
        points_l.push_back(innerPos);                   // center point
        points_l.push_back(getHCPoint(r, R, theta));

        //printf("%f\n", innerPos->x);

        points_h.clear();
        makeCycloid(&points_h, 1, r, R, 0, 1);

        render (points_h, points_r, points_l, points_R);
        glfwSwapBuffers (window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
