#include "ParticleSystem.h"
#include "SphereParticle.h"
#include "RectParticle.h"
#include "EarthGravityGenerator.h"
#include "ParticleLink.h"
#include "GroundContactGenerator.h"
#include "Camera.h"

#include "GLUT\glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const Color DEBUG_COLOR = Color(1,1,1);

struct Blob
{
	string name;
	SphereParticle* particle;
	Color color;
	Vector3 startPosition;
};

struct Block
{
	string name;
	RectParticle* particle;
	Color color;
	Vector3 startPosition;
};

class BlobGameSystem
{
private:
	void cleanUp();
	void createObjects();
	Blob* createBlob(string name, Color color, double size, double mass, Vector3 pos);
	Block* createBlock(string name, Color color, float width, float length, float height, double mass, Vector3 pos);
	void applyForces();
	void applyContacts();
	void drawDebug();
	void drawString(float x, float y, string text, Color color);

	bool mCanRestart;
	bool mDrawDebug;
	bool mCanDebug;
	vector<Blob*> mBlobs;
	vector<Block*> mBlocks;
	ParticleSystem* mParticleSystem;
	Camera* mCamera;
public:
	BlobGameSystem(Camera* camera);
	~BlobGameSystem();

	void initBlobs();
	void initBlocks();
	void press(char key, int isPressed);
	void update(float duration);
	void draw();
};