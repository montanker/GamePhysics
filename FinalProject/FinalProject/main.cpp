//Tim House
//Rigidbody simulation

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "Camera.h"
#include "RigidBodySystem.h"

#include "SphereParticle.h"
#include "CollisionPrimitive.h"
#include "CollisionDetection.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

Camera* camera;
RigidBodySystem* rigidBodyGame;

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
		case GLFW_KEY_R: rigidBodyGame->press('r',ispress); break;
		case GLFW_KEY_F: rigidBodyGame->press('f',ispress); break;
    }
}

void cleanUp()
{
	delete(camera);
	delete(rigidBodyGame);
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

	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	camera = new Camera(45.0f, ratio, 0.1f, powf(10,8), 5.0f, 5);
	camera->setPos(0, 80, 140);
	camera->setRot(30, 0, 0);
	glfwSetCursorPos(window, width/2, height/2);

	float sz = 5.0f;
	Cube cube = Cube(sz,sz,sz);
	rigidBodyGame = new RigidBodySystem(camera);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	float totalTime = 0;
	double speedup = 760320 * 5;
	int updateTicks = 1000;
	double speedupTick = speedup/(double) updateTicks;
	float speed = 1;

	while (!glfwWindowShouldClose(window))
	{
		float newTotalTime = glfwGetTime();
		float deltaTime = newTotalTime - totalTime;
		totalTime = newTotalTime;

		camera->update(window, (float)width, (float)height);

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		camera->draw();
		
		glMatrixMode(GL_MODELVIEW);

		rigidBodyGame->update(deltaTime);
		rigidBodyGame->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	cleanUp();

	exit(EXIT_SUCCESS);
}
