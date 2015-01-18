//Tim House
//Opengl codebase

//Using code samples from:
//https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
//http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLFW/glfw3.h>
#include "Shape.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int main(int argc, const char* argv[])
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1024, 768, "Testing Environment", NULL, NULL);

	if (!window)
	{
		fprintf( stderr, "Failed to create window\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	float size = 0.5f;

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		//glFrustum(-5.0f,5.0f,-5.0f,5.0f,0.001f,100.0f);
		gluPerspective(45.0f, ratio, 0.1f, 100.f);
		
		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -3.0f);
		glRotatef((float) glfwGetTime() * 50.f, 0.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			//Top face (y = 1.0f)    // Green
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(size, size, -size);
			glVertex3f(-size, size, -size);
			glVertex3f(-size, size, size);
			glVertex3f(size, size, size);
			//Bottom face (y = -1.0f)  // Orange
			glColor3f(size, 0.5f, 0.0f);
			glVertex3f(size, -size, size);
			glVertex3f(-size, -size, size);
			glVertex3f(-size, -size, -size);
			glVertex3f(size, -size, -size);
			// Front face  (z = 1.0f)
			glColor3f(1.0f, 0.0f, 0.0f);     // Red
			glVertex3f( size,  size, size);
			glVertex3f(-size,  size, size);
			glVertex3f(-size, -size, size);
			glVertex3f( size, -size, size);
			// Back face (z = -1.0f)
			glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
			glVertex3f( size, -size, -size);
			glVertex3f(-size, -size, -size);
			glVertex3f(-size,  size, -size);
			glVertex3f( size,  size, -size);
			// Left face (x = -1.0f)
			glColor3f(0.0f, 0.0f, 1.0f);     // Blue
			glVertex3f(-size,  size,  size);
			glVertex3f(-size,  size, -size);
			glVertex3f(-size, -size, -size);
			glVertex3f(-size, -size,  size);
			// Right face (x = 1.0f)
			glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
			glVertex3f(size,  size, -size);
			glVertex3f(size,  size,  size);
			glVertex3f(size, -size,  size);
			glVertex3f(size, -size, -size);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

	/*while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}*/
