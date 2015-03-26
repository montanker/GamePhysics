#include "BlobGameSystem.h"

BlobGameSystem::BlobGameSystem(Camera* camera)
{
	mCanRestart = true;
	mDrawDebug = true;
	mCanDebug = true;
	mCamera = camera;
	mBlobs = vector<Blob*>();
	mBlocks = vector<Block*>();
	mParticleSystem = new ParticleSystem();

	createObjects();
	initBlobs();

	mCamera->savePos();
}

BlobGameSystem::~BlobGameSystem()
{
	cleanUp();
}

void BlobGameSystem::cleanUp()
{
	delete(mParticleSystem);
}

void BlobGameSystem::createObjects()
{
	Blob* player = createBlob("Player", Color(0, 1, 0), 3, 1, Vector3(0, 20, 0));

	mBlobs.push_back(player);

	Block* floor = createBlock("Floor", Color(0.2f, 0.2f, 0.2f), 100, 100, 1, -1, Vector3(0,0,0));

	mBlocks.push_back(floor);

	applyForces();
	applyContacts();
}

Blob* BlobGameSystem::createBlob(string name, Color color, double size, double mass, Vector3 pos)
{
	Blob* blob = new Blob();
	blob->name = name;
	blob->color = color;
	blob->particle = new SphereParticle(size, mass, blob->color);
	blob->startPosition = pos;

	return blob;
}

Block* BlobGameSystem::createBlock(string name, Color color, float width, float length, float height, double mass, Vector3 pos)
{
	Block* block = new Block();
	block->name = name;
	block->color = color;
	block->particle = new RectParticle(width, length, height, mass, block->color);
	block->startPosition = pos;

	return block;
}

void BlobGameSystem::applyForces()
{
	//mParticleSystem->addParticle(mBlobs[0]->particle);
	EarthGravityGenerator* earthGrav = new EarthGravityGenerator();

	vector<Blob*>::iterator blobIt = mBlobs.begin();

	for (blobIt; blobIt != mBlobs.end(); ++blobIt)
	{
		mParticleSystem->applyForce((*blobIt)->particle, earthGrav);
	}

	
}

void BlobGameSystem::applyContacts()
{
	vector<Block*>::iterator blockIt = mBlocks.begin();

	for (blockIt; blockIt != mBlocks.end(); ++blockIt)
	{
		//mParticleSystem->addParticle((*blockIt)->particle);
	}
}

void BlobGameSystem::drawDebug()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	glColor4f(DEBUG_COLOR.r, DEBUG_COLOR.g, DEBUG_COLOR.b, DEBUG_COLOR.a);
	drawString(-1, 0.95f, "Hand over your coffee!", DEBUG_COLOR);
	drawString(-1, 0.9f, "Blob pos: "+mBlobs[0]->particle->getPosition().print(), DEBUG_COLOR);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void BlobGameSystem::drawString(float x, float y, string text, Color color)
{
	const char* temp = text.c_str();

	glRasterPos2f(x, y);

	char buf[300];
	sprintf(buf, temp);

	const char *p = buf;

	glColor4f(color.r, color.g, color.b, color.a);

	do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	while (*(++p));
}

void BlobGameSystem::initBlobs()
{
	vector<Blob*>::iterator blobIt = mBlobs.begin();

	for (blobIt; blobIt != mBlobs.end(); ++blobIt)
	{
		(*blobIt)->particle->setPosition((*blobIt)->startPosition);
		(*blobIt)->particle->setVelocity(Vector3(0,0,0));
	}
}

void BlobGameSystem::press(char key, int isPressed)
{
	if (key == 'r')
	{
		if (isPressed)
		{
			if (mCanRestart)
			{
				initBlobs();
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

void BlobGameSystem::update(float duration)
{
	mParticleSystem->update(duration);
}

void BlobGameSystem::draw()
{
	mParticleSystem->draw();
	if (mDrawDebug)
	{
		drawDebug();
	}
}