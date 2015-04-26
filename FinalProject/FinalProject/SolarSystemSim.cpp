#include "SolarSystemSim.h"

SolarSystemSim::SolarSystemSim(Camera* camera)
{
	mMassScale = 1;
	mPosScale = powf(10.0,9);
	mMoonPosScale = powf(10.1,9);
	mSizeScale = 5;
	mFocusPlanet = 0;
	mCanNext = true;
	mCanPrev = true;
	mCanRestart = true;
	mDrawDebug = true;
	mCanDebug = true;
	mCamera = camera;
	mPlanets = vector<Planet*>();
	mMoons = vector<Moon*>();
	mParticleSystem = new ParticleSystem();
	createPlanets();
	initPlanets();

	mCamera->savePos();
}
	
SolarSystemSim::~SolarSystemSim()
{
	cleanUp();
}

void SolarSystemSim::cleanUp()
{
	delete(mParticleSystem);
}

void SolarSystemSim::createPlanets()
{
	Planet* sun =     createPlanet("Sun", Color(1, 1, 0), 6, 1.989 * pow(10,30), 
		                          0, 0);
	Planet* mercury = createPlanet("Mercury", Color(0.25f, 0.25f, 0.25f), 1, 328.5 * pow(10, 21), 
								  5.79 * powf(10,10), -47400);
	Planet* venus =   createPlanet("Venus", Color(1, 0.75f, 0), 1.2f, 4.867 * pow(10, 24), 
								  1.082 * powf(10,11), -32400);
	Planet* earth =   createPlanet("Earth", Color(0, 0.75f, 0.75f), 1.3f, 5.972 * pow(10, 24), 
								  1.495 * powf(10, 11), -30000);
	Planet* mars =    createPlanet("Mars", Color(1, 0, 0), 1.2f, 639 * pow(10, 21), 
							      2.279 * powf(10, 11), -23000);
	Planet* jupiter = createPlanet("Jupiter", Color(1, 0.4f, 0), 3, 1.898 * pow(10, 27), 
							      7.785 * powf(10, 11), -12000);
	Planet* saturn =  createPlanet("Saturn", Color(0.75, 0.75f, 0), 2.75, 568.3 * pow(10, 24), 
							      1.433 * powf(10, 12), -10000);
	Planet* uranus =  createPlanet("Uranus", Color(0, 0.75f, 1), 1.75, 86.81 * pow(10, 24),
		                          2.877 * powf(10, 12), -6500);
	Planet* neptune = createPlanet("Neptune", Color(0, 0, 1), 1.75, 102.4 * pow(10, 24),
		                          4.503 * powf(10, 12), -5000);

	mPlanets.push_back(sun);
	mPlanets.push_back(mercury);
	mPlanets.push_back(venus);
	mPlanets.push_back(earth);
	mPlanets.push_back(mars);
	mPlanets.push_back(jupiter);
	mPlanets.push_back(saturn);
	mPlanets.push_back(uranus);
	mPlanets.push_back(neptune);

	Moon* earthMoon = createMoon("Earth's Moon", Color(0.75f, 0.75f, 0.75f), earth, 0.5f,
		                        7.347 * pow(10, 22), 3.844 * powf(10, 8), 1022);

	mMoons.push_back(earthMoon);

	applyGravity();
}

Planet* SolarSystemSim::createPlanet(string name, Color color, float size, double mass, double distance, double speed)
{
	Planet* planet = new Planet();
	planet->name = name;
	planet->color = color;
	planet->particle = new SphereParticle(size, mass * mMassScale, planet->color);
	planet->particle->setPositionScale(mPosScale);
	planet->particle->setSizeScale(mSizeScale);
	planet->startPosition = Vector3(distance, 0, 0);
	planet->startVelocity = Vector3(0, 0, speed);

	return planet;
}

Moon* SolarSystemSim::createMoon(string name, Color color, Planet* parent, float size, double mass, double distance, double speed)
{
	Moon* moon = new Moon();
	moon->name = name;
	moon->color = color;
	moon->planet = parent;
	moon->particle = new SphereParticle(size, mass * mMassScale, moon->color);
	moon->particle->setPositionScale(mMoonPosScale);
	moon->particle->setSizeScale(mSizeScale);

	Vector3 parentPos = moon->planet->startPosition;
	moon->startPosition = Vector3(parentPos.x + distance, parentPos.y, parentPos.z);
	Vector3 parentSpeed = moon->planet->startVelocity;
	moon->startVelocity = Vector3(parentSpeed.x, parentSpeed.y, parentSpeed.z + speed);

	return moon;
}

void SolarSystemSim::applyGravity()
{
	PlanetaryGravityGenerator* gravGen = new PlanetaryGravityGenerator();

	mParticleSystem = new ParticleSystem();
	vector<Planet*>::iterator planetIt = mPlanets.begin();
	//vector<Planet>::iterator otherPlanet;
	vector<Moon*>::iterator moonIt = mMoons.begin();

	for (planetIt; planetIt != mPlanets.end(); ++planetIt)
	{
		if ((*planetIt)->name != mPlanets[0]->name)
		{
			mParticleSystem->applyForce((*planetIt)->particle, mPlanets[0]->particle, gravGen);
		}
		/*otherPlanet = mPlanets.begin();
		for (otherPlanet; otherPlanet != mPlanets.end(); ++otherPlanet)
		{
			if (planetIt->name != otherPlanet->name)
			{
				mParticleSystem->applyForce(planetIt->particle, otherPlanet->particle, gravGen);
			}
		}*/
	}

	for (moonIt; moonIt != mMoons.end(); ++moonIt)
	{
		mParticleSystem->applyForce((*moonIt)->particle, (*moonIt)->planet->particle, gravGen);
		mParticleSystem->applyForce((*moonIt)->particle, mPlanets[0]->particle, gravGen);
	}
}

void SolarSystemSim::changeFocus(int planetNumber)
{
	if (mPlanets.size() < 1)
	{
		return;
	}

	mFocusPlanet = planetNumber;
	
	if (mFocusPlanet == 0)
	{
		mCamera->loadPos();
	}
}

void SolarSystemSim::drawDebug()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	if ((mFocusPlanet >= 1) && (mFocusPlanet <= 8))
	{
		drawString(-1, 0.95f, "Target: "+mPlanets[mFocusPlanet]->name, DEBUG_COLOR);
		drawString(-1, 0.9f, "Mass: "+mPlanets[mFocusPlanet]->particle->printMass(), DEBUG_COLOR);
		drawString(-1, 0.85f, "Velocity: "+mPlanets[mFocusPlanet]->particle->getVelocity().print(), DEBUG_COLOR);
		drawString(-1, 0.75f, "Use 1-8 to select planets", DEBUG_COLOR);
		drawString(-1, 0.7f, "Use 9 to select the moon", DEBUG_COLOR);
		drawString(-1, 0.65f, "Use 0 to zoom out", DEBUG_COLOR);
		drawString(-1, 0.6f, "Use R to reset", DEBUG_COLOR);
		drawString(-1, 0.55f, "Use F to toggle debug", DEBUG_COLOR);
	}
	else if (mFocusPlanet == 9)
	{
		drawString(-1, 0.95f, "Target: "+mMoons[0]->name, DEBUG_COLOR);
		drawString(-1, 0.9f, "Mass: "+mMoons[0]->particle->printMass(), DEBUG_COLOR);
		drawString(-1, 0.85f, "Velocity: "+mMoons[0]->particle->getVelocity().print(), DEBUG_COLOR);
		drawString(-1, 0.75f, "Use 1-8 to select planets", DEBUG_COLOR);
		drawString(-1, 0.7f, "Use 9 to select the moon", DEBUG_COLOR);
		drawString(-1, 0.65f, "Use 0 to zoom out", DEBUG_COLOR);
		drawString(-1, 0.6f, "Use R to reset", DEBUG_COLOR);
		drawString(-1, 0.55f, "Use F to toggle debug", DEBUG_COLOR);
	}
	else
	{
		drawString(-1, 0.95f, "Target: None", DEBUG_COLOR);
		drawString(-1, 0.85f, "Use 1-8 to select planets", DEBUG_COLOR);
		drawString(-1, 0.8f, "Use 9 to select the moon", DEBUG_COLOR);
		drawString(-1, 0.75f, "Use 0 to zoom out", DEBUG_COLOR);
		drawString(-1, 0.7f, "Use R to reset", DEBUG_COLOR);
		drawString(-1, 0.65f, "Use F to toggle debug", DEBUG_COLOR);
	}

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void SolarSystemSim::drawString(float x, float y, string text, Color color)
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

void SolarSystemSim::initPlanets()
{
	vector<Planet*>::iterator planetIt = mPlanets.begin();
	vector<Moon*>::iterator moonIt = mMoons.begin();

	for (planetIt; planetIt != mPlanets.end(); ++planetIt)
	{
		(*planetIt)->particle->setPosition((*planetIt)->startPosition);
		(*planetIt)->particle->setVelocity((*planetIt)->startVelocity);
	}

	for (moonIt; moonIt != mMoons.end(); ++moonIt)
	{
		(*moonIt)->particle->setPosition((*moonIt)->startPosition);
		(*moonIt)->particle->setVelocity((*moonIt)->startVelocity);
	}
}

void SolarSystemSim::press(char key, int isPressed)
{
	if (key == 'r')
	{
		if (isPressed)
		{
			if (mCanRestart)
			{
				initPlanets();
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

	if (key == '1')
	{
		changeFocus(1);
	}
	else if (key == '2')
	{
		changeFocus(2);
	}
	else if (key == '3')
	{
		changeFocus(3);
	}
	else if (key == '4')
	{
		changeFocus(4);
	}
	else if (key == '5')
	{
		changeFocus(5);
	}
	else if (key == '6')
	{
		changeFocus(6);
	}
	else if (key == '7')
	{
		changeFocus(7);
	}
	else if (key == '8')
	{
		changeFocus(8);
	}
	else if (key == '9')
	{
		changeFocus(9);
	}
	else if (key == '0')
	{
		changeFocus(0);
	}
}

void SolarSystemSim::update(float duration)
{
	mParticleSystem->update(duration);

	if (mFocusPlanet != 0)
	{
		Vector3 planetPos;
		float planetSize;
		if (mFocusPlanet < 9)
		{
			planetPos = mPlanets[mFocusPlanet]->particle->getPosition();
			planetSize = mPlanets[mFocusPlanet]->particle->getRadius();
		}
		else
		{
			planetPos = mMoons[0]->particle->getPosition();
			planetSize = mMoons[0]->particle->getRadius();
		}

		planetPos = Vector3(planetPos.x/mPosScale, planetPos.y/mPosScale, planetPos.z/mPosScale);
		//float planetSize = mPlanets[mFocusPlanet]->particle->getRadius();
		planetPos.y += planetSize*mSizeScale;

		Vector3 sunPos = mPlanets[0]->particle->getPosition();
		sunPos = Vector3(sunPos.x/mPosScale, sunPos.y/mPosScale, sunPos.z/mPosScale);

		Vector3 direction = sunPos - planetPos;
		direction.normalize();
		double scale = -(planetSize * mSizeScale * 15);
		planetPos.addScaledVector(direction, scale);

		mCamera->focus(planetPos, sunPos);
		//cout << "Focus: " << mPlanets[mFocusPlanet]->name << endl;
	}
}

void SolarSystemSim::draw()
{
	mParticleSystem->draw();
	if (mDrawDebug)
	{
		drawDebug();
	}
}