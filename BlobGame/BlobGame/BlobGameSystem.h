#include "ParticleSystem.h"
#include "SphereParticle.h"
#include "RectParticle.h"
#include "EarthGravityGenerator.h"
#include "AnchoredSpringGenerator.h"
#include "BungeeForceGenerator.h"
#include "ParticleLink.h"
#include "GroundContactGenerator.h"
#include "Camera.h"

#include "GLUT\glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const Color DEBUG_COLOR = Color(1,1,1);

struct Blob
{
	string name;
	SphereParticle* particle;
	Color color;
	Vector3 startPosition;
	float springConstant;
};

struct Arm
{
	string name;
	SphereParticle* particle;
	Color color;
	bool connected;
	Vector3 startPosition;
};

struct Block
{
	string name;
	RectParticle* particle;
	Color color;
	Vector3 startPosition;
};

const float ARM_GRAB_DIST = 20;

class BlobGameSystem
{
private:
	void cleanUp();
	void createLevel();
	Blob* createBlob(string name, Color color, double size, double mass, Vector3 pos);
	Arm* createArm(string name, Color color, double size, double mass, Vector3 pos);
	Block* createBlock(string name, Color color, float width, float length, float height, double mass, Vector3 pos);
	void createCube(string name, Color color, double size, double mass, double length, Vector3 pos);
	void createPyramid(string name, Color color, double size, double mass, double length, Vector3 pos);
	void createDiamond(string name, Color color, double size, double mass, double length, Vector3 pos);
	void createSpring(Color color, double size, double mass, float springConstant, Vector3 pos);
	void createBungee(Color color, double size, double mass, float springConstant, Vector3 pos);
	void applyForces();
	void applyContacts();
	void createRod(Particle* particle1, Particle* particle2, Color color, double length);
	void createCable(Particle* particle1, Particle* particle2, Color color, double restitution, double length);
	void movePlayer(Vector3 moveVelocity);
	void drawDebug();
	void drawString(float x, float y, string text, Color color);

	bool mCanRestart;
	bool mDrawDebug;
	bool mCanDebug;
	Blob* mPlayer;
	Blob* mSpring;
	Blob* mBungee;
	float mPlayerSpeed;
	int arms;
	vector<Blob*> mBlobs;
	vector<Block*> mBlocks;
	vector<Arm*> mArms;
	ParticleSystem* mParticleSystem;
	Camera* mCamera;
public:
	BlobGameSystem(Camera* camera);
	~BlobGameSystem();

	void initBlobs();
	void initArms();
	void initBlocks();
	void press(char key, int isPressed);
	void update(float duration);
	void draw();
};