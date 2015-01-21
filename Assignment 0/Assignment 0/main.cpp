//Tim House
//Opengl codebase

//Using code samples from:
//https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
//http://r3dux.org/2011/05/simple-opengl-keyboard-and-mouse-fps-controls/

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "Camera.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

Camera* camera;

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

	int ispress = action != GLFW_RELEASE;
    switch(key) {
        case GLFW_KEY_W: camera->press('w',ispress); break;
        case GLFW_KEY_S: camera->press('s',ispress); break;
        case GLFW_KEY_A: camera->press('a',ispress); break;
        case GLFW_KEY_D: camera->press('d',ispress); break;
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

	Cube* cube = new Cube(1.0f, 1.0f, 1.0f);
	cube->setPos(0.0f, 0.0f, -3.0f);

	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	camera = new Camera(45.0f, ratio, 0.1f, 100.0f, 5.0f, 5.0f);
	glfwSetCursorPos(window, width/2, height/2);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	float size = 0.5f;

	while (!glfwWindowShouldClose(window))
	{
		camera->update(window, width, height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		camera->draw();
		//glRotatef((float) glfwGetTime() * 50.f, 0.0f, 1.0f, 1.0f);
		
		glMatrixMode(GL_MODELVIEW);
		cube->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
