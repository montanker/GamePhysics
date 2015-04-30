#include "RigidBody.h"

class RbForceGenerator
{
public:
	virtual void updateForce(RigidBody *body, double duration) = 0;
};

class RbGravity : public RbForceGenerator
{
    Vector3 gravity;

public:
    RbGravity(const Vector3 &grav);

    virtual void updateForce(RigidBody *body, double duration);
};

class RbSpring : public RbForceGenerator
{
private:
    Vector3 connectionPoint;
    Vector3 otherConnectionPoint;

    RigidBody *otherBody;
    double springConst;
    double restitution;

public:
    RbSpring(const Vector3 &localConnectionPt, RigidBody *other, const Vector3 &otherConnectionPt,
           double springConstant, double restLength);

    virtual void updateForce(RigidBody *body, double duration);
};