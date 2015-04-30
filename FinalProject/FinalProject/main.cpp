//Tim House
//Dominoes simulation

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "Camera.h"
//#include "BlobGameSystem.h"
#include "RigidBodySystem.h"

#include "SphereParticle.h"
#include "CollisionPrimitive.h"
#include "CollisionDetection.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

Camera* camera;
//BlobGameSystem* blobGame;
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
		/*case GLFW_KEY_W: blobGame->press('w',ispress); break;
		case GLFW_KEY_S: blobGame->press('s',ispress); break;
		case GLFW_KEY_A: blobGame->press('a',ispress); break;
		case GLFW_KEY_D: blobGame->press('d',ispress); break;*/
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
	//blobGame = new BlobGameSystem(camera);
	rigidBodyGame = new RigidBodySystem(camera);

	double spos = 60.0;

	int nrb = 2;
	RigidBody rb[] = { RigidBody(), RigidBody(), RigidBody()};
	rb[0].setPosition(Vector3(spos,10.0,0.0));
	rb[0].setVelocity(Vector3(-20.0,0.0,0.0));

	CollisionData cdata = CollisionData();
	int ncprim = 5;
	CollisionPrimitive *cprim[] =
	{
		new CollisionSphere(5.0,Color(0,0,1.0f)),
		new CollisionSphere(20.0,Color(1,1,1)),
		new CollisionSphere(2.0,Color(1,1,0)),
		new CollisionBox(Vector3(1,1,1),Color(1.0f,0,1.0f)),
		new CollisionPlane(Vector3(0,1,0),Color(0.2f,0.2f,0.2f))
	};

	cprim[0]->body = &rb[0];

	cprim[2]->body = &rb[0];
	cprim[2]->offset.setAxisVector(3,-8.6,0,0);

    cprim[3]->body = &rb[0];
	cprim[3]->offset.setAxisVector(3,0,10,0);

	cprim[1]->body = &rb[1];
	//cprim[1]->setPosition(Vector3(0, 25, 0));

	cprim[4]->body = &rb[2];

	// Test boxAndSphere
	/*for(int s = 0;s<ncprim;s++)
	{
		cprim[s]->calculateInternals();
	}
	cdata.reset(1);
	unsigned hit = cprim[1]->DetectCollision(cprim[3],&cdata);
	cout << "HIT: "<<hit<<endl;*/


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

		/*for(int s = 0;s<nrb;s++)
		{
			rb[s].integrate(deltaTime);
		}*/

		rigidBodyGame->update(deltaTime);
		/*for(int s = 0;s<ncprim;s++)
		{
			cprim[s]->calculateInternals();
			cprim[s]->integrate(deltaTime);
		}*/

		unsigned hit = 0;
		cdata.reset(1);
		// boxAndSphere maybe not working?
		for(int s=0;s<ncprim;s++) 
		{
			if(s == 1) 
			{
				continue;
			}
			if (s == 3)
			{
				continue;
			}
			hit += cprim[1]->DetectCollision(cprim[s],&cdata);
		}
		cprim[1]->color = hit?Color(1.0f,0,0):Color(0,1.0f,0);

		rigidBodyGame->draw();
		/*for(int s=0;s<ncprim;s++)
		{
			cprim[s]->draw();
		}*/

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	cleanUp();

	exit(EXIT_SUCCESS);
}
