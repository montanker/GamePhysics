#include "Camera.h"
#include "Color.h"
#include "RigidBodyForceRegistry.h"
#include "CollisionPrimitive.h"
#include "CollisionDetection.h"

#include "GLUT\glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const Color DEBUG_COLOR = Color(1,1,1);
const string DEBUG_TEXT = "Hand over your coffee!";

struct Ball
{
	string name;
	CollisionPrimitive* col;
	RigidBody* body;
	Vector3 startPos;
	Vector3 startVel;
};

struct BodyRegistration
{
    RigidBody *body;
    BodyRegistration * next;
};

struct ContactGenRegistration
{
    ContactGenerator *gen;
    ContactGenRegistration *next;
};

class RigidBodySystem
{
private:
	void cleanUp();
	void createLevel();
	Ball* createBall(string name, Color color, double radius, double mass, Vector3 pos = Vector3(), Vector3 vel = Vector3());
	void applyForces();
	void drawDebug();
	void drawString(float x, float y, string text, Color color);
	unsigned generateContacts();

	bool mCanRestart;
	bool mDrawDebug;
	bool mCanDebug;
	unsigned mMaxContacts;
	vector<Ball*> mBalls;
	RbForceRegistry mForceReg;
	ContactGenRegistration* mFirstContactGen;
	Contact* mContacts;
	ContactResolver mResolver;
	Ball* mStaticBall;
	Camera* mCamera;

public:
	RigidBodySystem(Camera* camera);
	~RigidBodySystem();
	
	void initBalls();
	void press(char key, int isPressed);
	void update(float duration);
	void draw();
};