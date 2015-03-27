#include "ParticleContact.h"

void ParticleContact::init()
{
	mRestitution = 0;
	mPenetration = 0;
	mParticleMovement[0] = Vector3(0,0,0);
	mParticleMovement[1] = Vector3(0,0,0);
	mContactNormal = Vector3(0,0,0);
	drawLine = false;
	color = CONTACT_COLOR;
}

void ParticleContact::draw()
{
	if (!drawLine)
	{
		return;
	}

	glLoadIdentity();
	glPushMatrix();
	glLineWidth(CONTACT_WIDTH);
	glBegin(GL_LINES);
		glColor3f(color.r, color.g, color.b);
		Vector3 pos1 = mParticle[0]->getPosition();
		Vector3 pos2 = mParticle[1]->getPosition();
		glVertex3f(pos1.x, pos1.y, pos1.z);
		glVertex3f(pos2.x, pos2.y, pos2.z);
	glEnd();
	glPopMatrix();
}

void ParticleContact::resolve(float duration)
{
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

double ParticleContact::calculateSeparatingVelocity()
{
	Vector3 relativeVelocity = mParticle[0]->getVelocity();
	
	if (mParticle[1])
	{
		relativeVelocity = relativeVelocity - mParticle[1]->getVelocity();
	}

	return relativeVelocity.dotProduct(mContactNormal);
}

void ParticleContact::resolveVelocity(float duration)
{
	double separatingVelocity = calculateSeparatingVelocity();
	if(separatingVelocity > 0)  //Separating or stationary
	{
		return;
	}

	double totalInverseMass = mParticle[0]->getInverseMass();
	if (mParticle[1])
	{
		totalInverseMass += mParticle[1]->getInverseMass();
	}
	if (totalInverseMass <= 0)  //Infinite mass
	{
		return;
	}

	double newSepVelocity = -separatingVelocity * mRestitution;
	Vector3 accCausedVelocity = mParticle[0]->getAcceleration();
	if (mParticle[1])
	{
		accCausedVelocity = accCausedVelocity - mParticle[1]->getAcceleration();
	}

	double accCausedSepVelocity = accCausedVelocity.dotProduct(mContactNormal)*duration; 
	if (accCausedSepVelocity < 0)
	{
		newSepVelocity += mRestitution * accCausedSepVelocity;
		if (newSepVelocity < 0)
		{
			newSepVelocity = 0;
		}
	}

	double deltaVelocity = newSepVelocity - separatingVelocity;

	double impulse = deltaVelocity / totalInverseMass;

	Vector3 impulsePerIMass = mContactNormal * impulse;

	mParticle[0]->setVelocity(mParticle[0]->getVelocity() + impulsePerIMass * mParticle[0]->getInverseMass());
	if (mParticle[1])
	{
		mParticle[1]->setVelocity(mParticle[1]->getVelocity() + impulsePerIMass * -mParticle[1]->getInverseMass());
	}
}

void ParticleContact::resolveInterpenetration(float duration)
{
	if (mPenetration <= 0)
	{
		return;
	}

	double totalInverseMass = mParticle[0]->getInverseMass();
	if (mParticle[1])
	{
		totalInverseMass += mParticle[1]->getInverseMass();
	}

	if (totalInverseMass <= 0)
	{
		return;
	}

	Vector3 movePerIMass = mContactNormal * (mPenetration / totalInverseMass);

	mParticleMovement[0] = movePerIMass * mParticle[0]->getInverseMass();
	if (mParticle[1])
	{
		mParticleMovement[1] = movePerIMass * -mParticle[1]->getInverseMass();
	}
	else
	{
		mParticleMovement[1] = Vector3(0,0,0);
	}

	mParticleMovement[0] = mParticleMovement[0] * duration;
	mParticleMovement[1] = mParticleMovement[1] * duration;

	mParticle[0]->setPosition(mParticle[0]->getPosition() + mParticleMovement[0]);
	if (mParticle[1])
	{
		mParticle[1]->setPosition(mParticle[1]->getPosition() + mParticleMovement[1]);
	}
}