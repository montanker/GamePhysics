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
	float mMass;
	float mDamping;
	Vector3 mPosition;
	Vector3 mVelocity;
	Vector3 mAcceleration; 
	Vector3 mForceAccumulation;
public:
	Particle(){};

	void update(float duration);
	void addForce(Vector3 addedForce);
	float getInverseMass();
	bool isDead();

	//Getters
	float getAge()            {return mAge;};
	float getMass()           {return mMass;};
	float getDamping()        {return mDamping;};
	Vector3 getPosition()     {return mPosition;};
	Vector3 getVelocity()     {return mVelocity;};
	Vector3 setAcceleration() {return mAcceleration;};
	
	//Setters
	void setAge(float age)                     {mAge = age;};
	void setMass(float mass)                   {mMass = mass;};
	void setDamping(float damping)             {mDamping = damping;};
	void setPosition(Vector3 position)         {mPosition = position;};
	void setVelocity(Vector3 velocity)         {mVelocity = velocity;};
	void setAcceleration(Vector3 acceleration) {mAcceleration = acceleration;};
	
	virtual void draw() = 0;
};