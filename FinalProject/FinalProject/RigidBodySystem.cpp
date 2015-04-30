#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem(Camera* camera)
{
	mCanRestart = true;
	mDrawDebug = true;
	mCanDebug = true;
	mCamera = camera;

	mMaxContacts = 99;
	mContacts = new Contact[mMaxContacts];
	mFirstContactGen = NULL;
	mResolver = ContactResolver(99);

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
	Ball* staticBall = createBall("StaticBall", Color(1,0,0), 10, -1);
	Ball* fallingBall = createBall("FallingBall", Color(0,1,0), 5, 1, Vector3(20, 0, 0));
	
	mBalls.push_back(staticBall);
	mBalls.push_back(fallingBall);

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
		mForceReg.add((*ballIt)->body, gravForce);
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
	drawString(-1, drawPos, DEBUG_TEXT, DEBUG_COLOR);
	drawString(-1, drawPos-=spacing, mBalls.at(1)->body->getPosition().print(), mBalls.at(1)->col->color);
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

unsigned RigidBodySystem::generateContacts()
{
    unsigned limit = maxContacts;
    Contact *nextContact = contacts;

    ContactGenRegistration * reg = firstContactGen;
    while (reg)
    {
        unsigned used = reg->gen->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // We've run out of contacts to fill. This means we're missing
        // contacts.
        if (limit <= 0) break;

        reg = reg->next;
    }

    // Return the number of contacts used.
    return maxContacts - limit;
}

void RigidBodySystem::initBalls()
{
	vector<Ball*>::iterator ballIt = mBalls.begin();

	for (ballIt; ballIt != mBalls.end(); ++ballIt)
	{
		(*ballIt)->col->setPosition((*ballIt)->startPos);
		(*ballIt)->col->setVelocity((*ballIt)->startVel);
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
