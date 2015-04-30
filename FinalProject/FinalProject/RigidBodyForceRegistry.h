#include "RigidBodyForces.h"
#include <vector>

class RbForceRegistry
{
protected:
    struct RbForceRegistration
    {
        RigidBody *body;
        RbForceGenerator *fg;
    };

    vector<RbForceRegistration> registrations;

public:
    void add(RigidBody* body, RbForceGenerator *fg);
    void remove(RigidBody* body, RbForceGenerator *fg);
    void clear();
    void updateForces(double duration);
};