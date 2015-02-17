#include "ParticleSystem.h"
#include "SphereParticle.h"
#include "PlanetaryGravityGenerator.h"
#include "Camera.h"

#include "GLUT\glut.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const Color DEBUG_COLOR = Color(1,1,1);

struct Planet
{
	string name;
	SphereParticle* particle;
	Color color;
	Vector3 startPosition;
	Vector3 startVelocity;
};

struct Moon
{
	string name;
	SphereParticle* particle;
	Planet* planet;
	Color color;
	Vector3 startPosition;
	Vector3 startVelocity;
};

class SolarSystemSim
{
private:
	void cleanUp();
	void createPlanets();
	Planet* createPlanet(string name, Color color, float size, double mass, double distance, double speed);
	Moon* createMoon(string name, Color color, Planet* parent, float size, double mass, double distance, double speed);
	void applyGravity();
	void changeFocus(int planetNumber);
	void drawDebug();
	void drawString(float x, float y, string text, Color color);

	double mMassScale;
	double mPosScale;
	double mMoonPosScale;
	double mSizeScale;
	int mFocusPlanet;
	bool mCanNext;
	bool mCanPrev;
	bool mCanRestart;
	bool mDrawDebug;
	bool mCanDebug;
	vector<Planet*> mPlanets;
	vector<Moon*> mMoons;
	ParticleSystem* mParticleSystem;
	Camera* mCamera;
public:
	SolarSystemSim(Camera* camera);
	~SolarSystemSim();

	void initPlanets();
	void press(char key, int isPressed);
	void update(float duration);
	void draw();
};