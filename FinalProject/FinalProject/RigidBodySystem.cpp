#include "Contacts.h"
#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem(Camera* camera)
{
	mCanRestart = true;
	mDrawDebug = true;
	mCanDebug = true;
	mCamera = camera;

	srand(time(NULL));

	mMaxContacts = 99;
	mContacts = new Contact[mMaxContacts];
	mFirstContactGen = NULL;
	mResolver = ContactResolver(99);
	mResolver.setIterations(99,99);
	mResolver.setEpsilon(.01f, 0.01f);

	mBalls = vector<Ball*>();
	mForceReg = RbForceRegistry();

	createLevel();
	applyForces();

	initBalls();

	mCamera->savePos();
}

RigidBodySystem::~RigidBodySystem()
{
	cleanUp();
}

void RigidBodySystem::cleanUp()
{
	mForceReg.clear();
}

void RigidBodySystem::createLevel()
{
	Ball* staticBall   = createBall("StaticBall", Color(1,0,0), 15, 99999);
	Ball* fallingBall1 = createBall("FallingBall1", Color(0,1,0), 2, 2, Vector3(0, 20, 0));
	Ball* fallingBall2 = createBall("FallingBall2", Color(0,0,1), 2, 2, Vector3(0, 40, 0));
	Ball* fallingBall3 = createBall("FallingBall3", Color(1,1,0), 2, 2, Vector3(0, 20, 0));
	Ball* fallingBall4 = createBall("FallingBall4", Color(1,0,1), 2, 2, Vector3(0, 20, 0));
	
	mBalls.push_back(staticBall);
	mBalls.push_back(fallingBall1);
	mBalls.push_back(fallingBall2);
	mBalls.push_back(fallingBall3);
	mBalls.push_back(fallingBall4);

	mStaticBall = staticBall;

	applyForces();
}

Ball* RigidBodySystem::createBall(string name, Color color, double radius, double mass, Vector3 pos, Vector3 vel)
{
	Ball* ball = new Ball();
	ball->name = name;
	ball->col = new CollisionSphere(radius, color);
	ball->col->body = new RigidBody();
	ball->body = ball->col->body;
	ball->body->setMass(mass);
	ball->startPos = pos;
	ball->startVel = vel;

	return ball;
}

void RigidBodySystem::applyForces()
{
	RbGravity* gravForce = new RbGravity(Vector3(0,-9.8,0));

	vector<Ball*>::iterator ballIt = mBalls.begin();

	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		if ((*ballIt)->name != "StaticBall")
		{
			mForceReg.add((*ballIt)->body, gravForce);
		}
	}

	/*RbSpring* springForce = new RbSpring(Vector3(0,0,0),mStaticBall->body, Vector3(0,0,0), 0.5f, 40);
	mForceReg.add(mBalls.at(1)->body, springForce);*/
}

void RigidBodySystem::drawDebug()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	//glColor4f(DEBUG_COLOR.r, DEBUG_COLOR.g, DEBUG_COLOR.b, DEBUG_COLOR.a);
	float drawPos = 0.95f;
	float spacing = 0.05f;
	for (int i=1; i < mBalls.size(); i++)
	{
		if (i>1)
		{
			drawPos-=spacing;
		}
		string text = "Ball "+ std::to_string(i) + " Position: ";
		text +=  mBalls.at(i)->body->getPosition().print();
		drawString(-1, drawPos, text, mBalls.at(i)->col->color);
	}
	drawString(-1, drawPos-=spacing, "WASD to move", DEBUG_COLOR);
	drawString(-1, drawPos-=spacing, "Mouse to look", DEBUG_COLOR);
	drawString(-1, drawPos-=spacing, "R to reset", DEBUG_COLOR);
	drawString(-1, drawPos-=spacing, "F to toggle debug", DEBUG_COLOR);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RigidBodySystem::drawString(float x, float y, string text, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	const char* temp = text.c_str();

	glRasterPos2f(x, y);

	char buf[300];
	sprintf(buf, temp);

	const char *p = buf;

	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	while (*(++p));
}

void RigidBodySystem::generateContacts(double duration)
{
	CollisionData data = CollisionData();
	data.friction = 0.5f;
	data.restitution = 0.5f;
	data.reset(99);

	vector<Ball*>::iterator ballIt = mBalls.begin();
	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		vector<Ball*>::iterator ballIt2 = mBalls.begin();
		for (ballIt2; ballIt2 != mBalls.end(); ++ballIt2)
		{
			if (ballIt != ballIt2)
			{
				(*ballIt)->col->DetectCollision((*ballIt2)->col, &data);
				
			}
		}
	}

	if (data.contactCount > 0)
	{
		//cout << data.contactCount << endl;
		mResolver.resolveContacts(data.contactArray, data.contactCount, duration);
	}
	
}

void RigidBodySystem::initBalls()
{
	vector<Ball*>::iterator ballIt = mBalls.begin();

	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		if ((*ballIt)->name != "StaticBall")
		{
			double height = rand() % 20 + 20;
			double randX = rand() % 10 - 5;
			double randZ = rand() % 10 - 5;
			(*ballIt)->col->setPosition(Vector3(randX, height, randZ));
			(*ballIt)->col->setVelocity((*ballIt)->startVel);
		}
		else
		{
			(*ballIt)->col->setPosition((*ballIt)->startPos);
			(*ballIt)->col->setVelocity((*ballIt)->startVel);
		}
	}
}

void RigidBodySystem::press(char key, int isPressed)
{
	if (key == 'r')
	{
		if (isPressed)
		{
			if (mCanRestart)
			{
				initBalls();
				mCanRestart = false;
			}
		}
		else
		{
			mCanRestart = true;
		}
	}

	if (key == 'f')
	{
		if (isPressed)
		{
			if (mCanDebug)
			{
				mDrawDebug = !mDrawDebug;
				mCanDebug = false;
			}
		}
		else
		{
			mCanDebug = true;
		}
	}
}

void RigidBodySystem::update(float duration)
{
	mForceReg.updateForces(duration);

	vector<Ball*>::iterator ballIt = mBalls.begin();

	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		(*ballIt)->col->calculateInternals();
		(*ballIt)->col->integrate(duration);
	}

	generateContacts(duration);
}

void RigidBodySystem::draw()
{
	vector<Ball*>::iterator ballIt = mBalls.begin();

	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		(*ballIt)->col->draw();
	}

	if (mDrawDebug)
	{
		drawDebug();
	}
}
