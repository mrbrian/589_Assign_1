#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) \
 || defined(__TOS_WIN__) || defined(__WINDOWS__)
/* Compiling for Windows */
#ifndef __WINDOWS__
#define __WINDOWS__
#endif
#  include <windows.h>
#endif/* Predefined Windows macros */
#define GLFW_DLL
#include<GLFW/glfw3.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<iostream>

void render () {
	glEnable (GL_DEPTH_TEST);
	//glEnable (GL_LIGHTING);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Functions for changing transformation and projection matrices
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.5f, 0.5f, 0.0f);
	glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	glScalef(0.5f, 0.5f, 1.0f);

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
	glEnd ();
}

int main () {
	if (!glfwInit()) {
		std::cout << "Could not initialize.\n";
		return 1;
	}

	GLFWwindow *window = glfwCreateWindow (640, 480, "My Window", NULL, NULL);
	if (!window) {
		std::cout << "Could not create window.\n";
		glfwTerminate();
		return 1;
	}


	glfwMakeContextCurrent (window);
	while (!glfwWindowShouldClose (window)) {
		render ();
		glfwSwapBuffers (window);
		glfwPollEvents();
	}


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

