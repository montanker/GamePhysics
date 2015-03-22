#pragma once

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <GLFW/glfw3.h>
#include "Vector3.h"
#include <assert.h>

class Particle
{
protected:
	void init();
	void integrate(float duration);
	void handleAge(float duration);
	void clearAccumulator();

	float mAge;
	double mMass;
	double mDamping;
	double mPosScale;
	double mSizeScale;
	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mAcceleration; 
	Vector3 mForceAccumulation;
public:
	Particle(){};

	void update(float duration);
	void addForce(Vector3 addedForce);
	double getInverseMass();
	bool isDead();

	//Prints
	string printMass();

	//Getters
	float getAge()            {return mAge;};
	double getMass()          {return mMass;};
	double getDamping()       {return mDamping;};
	double getPositionScale() {return mPosScale;};
	double getSizeScale()     {return mSizeScale;};
	Vector3 getPosition()     {return mPosition;};
	Vector3 getVelocity()     {return mVelocity;};
	Vector3 getAcceleration() {return mAcceleration;};
	
	//Setters
	void setAge(float age)                     {mAge = age;};
	void setMass(double mass)                  {mMass = mass;};
	void setDamping(double damping)            {mDamping = damping;};
	void setPositionScale(double posScale)     {mPosScale = posScale;};
	void setSizeScale(double sizeScale)        {mSizeScale = sizeScale;};
	void setPosition(Vector3 position)         {mPosition = position;};
	void setVelocity(Vector3 velocity)         {mVelocity = velocity;};
	void setAcceleration(Vector3 acceleration) {mAcceleration = acceleration;};
	
	virtual void draw() = 0;
};