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
#include "point3D.h"
#include <vector>
#include <stdio.h>

#define RAD_STEP	M_PI / 180	// step size for rendering circles/hypocycloid
#define STEP        0.01		// step size for modifying values via keyboard

bool refresh = true;	// force
bool animate;			// true = playing an animation, false = display the hypocycloid

float R = 1;        // outer circle radius
float r = 0.25;		// inner circle radius
float scale = 1;	// scene scale

int cycles = 1;		// number of cycles for hypocycloid
int rotation = 0;	// scene rotation in degrees

float FRAMETIME = 1.0f / 60;	// time per frame at 60 FPS

/*  Iterates through a point list and passes each point through glVertex3f  */
void drawPoints(std::vector<Point3D*> points)
{
    for (std::vector<Point3D*>::iterator it = points.begin(); it != points.end(); ++it)
    {
        Point3D *pt = (*it);

        glVertex3f(pt->x, pt->y, pt->z);
    }
}

/*  Passes off each of the point lists for rendering as lines */
void render(std::vector<Point3D*> p_h, std::vector<Point3D*> p_R, std::vector<Point3D*> p_r, std::vector<Point3D*> p_l)
{
    glEnable (GL_DEPTH_TEST);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Functions for changing transformation and projection matrices
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, 0.0f);
    glRotatef(rotation, 0.0f, 0.0f, 1.0f);
    glScalef(scale, scale, scale);

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

/*   Gets a single point of the hypocycloid defined by the inner circle radius r, outer circle radius R, and angle theta   */
Point3D *getHCPoint(float r, float R, double theta)
{
    double x = (R - r)*cos(theta) + r*cos(((R - r) / r)*theta);
    double y = (R - r)*sin(theta) - r*sin(((R - r) / r)*theta);

    Point3D *result = new Point3D(x, y, 0);

    return result;
}


/*  Generates points of a circle at the given position with given radius  */
void circlePoints(std::vector<Point3D*> *points, float radius, Point3D pos)
{
    for (double theta = 0; theta < 2 * M_PI; theta += RAD_STEP)
    {
        double x = radius * cos(theta) + pos.x;
        double y = radius * sin(theta) + pos.y;

        points->push_back(new Point3D(x, y, 0));
    }
}

/*  Iterates a full 360 degrees (2pi), and populates the given list with the hypocycloid points  */
void makeCycloid(std::vector<Point3D*> *points, int n, float r, float R, float rot, float s)
{
    for (int cycle = 0; cycle < n; cycle++)
    {
		for (float theta = 0; theta < 2 * M_PI; theta += RAD_STEP)
        {
			float offset = 2 * M_PI * cycle;
            points->push_back(getHCPoint(r, R, offset + theta));
        }
    }
}

/*  Returns the position of the inner circle (radius r) with the given outer radius R and theta  */
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
		case GLFW_KEY_ESCAPE:		// exit the application 
			printf("Exiting..\n", r);
			glfwSetWindowShouldClose(window, GL_TRUE);
			return;
		case GLFW_KEY_MINUS:		// increase the number of cycles  (doesnt cause a refresh, the return statement skips it)
			if (cycles > 1)
				cycles--;
			printf("Cycles: %d\n", cycles);
			return;
		case GLFW_KEY_EQUAL:		// decrease the number of cycles
			cycles++;
			printf("Cycles: %d\n", cycles);
			return;
		case GLFW_KEY_A:			// rotate clockwise
			rotation--;
			printf("Rotation: %.2f\n", rotation);
			return;
		case GLFW_KEY_D:			// rotate counterclockwise
			rotation++;
			printf("Rotation: %.2f\n", rotation);
			return;
		case GLFW_KEY_W:			// increase scale
			scale += STEP;
			printf("Scale: %.2f\n", rotation);
			return;
		case GLFW_KEY_S:			// decrease scale 
			if (scale > 0)
				scale -= STEP;
			printf("Scale: %.2f\n", rotation);
			return;
		case GLFW_KEY_LEFT:			// decrease inner circle radius  (causes refresh via break statement)
			if (r > STEP)
				r -= STEP;
			printf("Inner Radius: %.2f\n", r);
			break;
		case GLFW_KEY_RIGHT:		// increase inner circle radius  (causes refresh)
			if ((r + STEP) < R)
				r += STEP;
			printf("Inner Radius: %.2f\n", r);
			break;
		case GLFW_KEY_DOWN:			// decrease outer circle radius  (causes refresh)
			if (R > STEP)
				R -= STEP;
			printf("Outer Radius: %.2f\n", R);
			break;
		case GLFW_KEY_UP:			// increase outer circle radius  (causes refresh)
			R += STEP;
			printf("Outer Radius: %.2f\n", R);
			break;
		case GLFW_KEY_SPACE:		// start animation  (causes refresh)
			animate = true;
			printf("Starting animation.\n", rotation);
			break;
		default:
			return;					// default, no refresh
		}
		refresh = true;		// clear & update the scene point lists
	}
}

/*  Frees allocated memory and clear a vector list */
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

/*  Populates the point lists  */
void hypocycloidPoints(std::vector<Point3D*> *points_h,
					std::vector<Point3D*> *points_R, 
					std::vector<Point3D*> *points_r, 
					std::vector<Point3D*> *points_l)
{
	// Clear old data
	clearVector(points_h);
	clearVector(points_R);
	clearVector(points_r);
	clearVector(points_l);

	// Generate outer circle
	circlePoints(points_R, R, Point3D(0, 0, 0));

	// Generate/store hypocycloid points
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
	const float SPEED = 2;
	float &theta = *p_theta;	// update the value of ref parameter p_theta via theta

	if (refresh)		// some parameter changed, so start the animation over
	{
		theta = 0;
		clearVector(points_h);
		refresh = false;
	}

	if (theta < M_PI * 2 * cycles)
	{
		clearVector(points_R);		// clearing old points
		clearVector(points_r);
		clearVector(points_l);

		circlePoints(points_R, R, Point3D(0, 0, 0));	// generate outer circle

		Point3D *innerPos = innerPath(r, R, theta);		// get the center position of the inner circle
		circlePoints(points_r, r, *innerPos);			// generate inner circle

		points_l->push_back(innerPos);                  // center point of inner circle
		points_l->push_back(getHCPoint(r, R, theta));	// fixed point of inner circle
		points_h->push_back(getHCPoint(r, R, theta));

		theta += FRAMETIME * SPEED;		// increment theta
	}
	else
	{
		animate = false;	// animation is over
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
    glfwSetKeyCallback(window, key_callback);	// setup input callback 

	// Separate point lists for each screen element

    std::vector<Point3D*> points_h;		// hypocycloid
    std::vector<Point3D*> points_R;		// large circle
    std::vector<Point3D*> points_r;		// small circle
    std::vector<Point3D*> points_l;		// line of small circle
	
	float theta = 0;		// theta of the current animation
	double currTime = 0;	// time
	double prevTime = 0;
	float timer = 0;

    while (!glfwWindowShouldClose (window))
    {
		// Time code for making updates at 60 FPS
		currTime = glfwGetTime();
		double elapsedTime = currTime - prevTime;
		timer -= elapsedTime;
		prevTime = currTime;

		if (timer <= 0)		// if it's time to render a new frame..
		{
			timer = FRAMETIME;
			if (!animate)
			{
				hypocycloidPoints(&points_h, &points_R, &points_r, &points_l);		// draw the whole hypocycloid, populates the point list 
			}
			else
			{
				updateAnim(&theta, &points_h, &points_R, &points_r, &points_l);		// draw the animation, appending to the previous point list
			}
			render(points_h, points_R, points_r, points_l);		// render the scene
			glfwSwapBuffers(window);
		}

		glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
