#include "RigidBodyForceRegistry.h"

void RbForceRegistry::updateForces(double duration)
{
    vector<RbForceRegistration>::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
        i->fg->updateForce(i->body, duration);
    }
}

void RbForceRegistry::add(RigidBody *body, RbForceGenerator *fg)
{
    RbForceRegistry::RbForceRegistration registration;
    registration.body = body;
    registration.fg = fg;
    registrations.push_back(registration);
}

void RbForceRegistry::remove(RigidBody* body, RbForceGenerator *fg)
{
	vector<RbForceRegistration>::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
		if ((i->body == body) && (i->fg == fg))
		{
			registrations.erase(i);
			return;
		}
    }
}

void RbForceRegistry::clear()
{
	/*vector<RbForceRegistration>::iterator i = registrations.begin();
    for (; i != registrations.end(); i++)
    {
    }*/
	registrations.clear();
}