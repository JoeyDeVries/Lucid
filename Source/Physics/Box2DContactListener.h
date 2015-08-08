#ifndef BOX_2D_CONTACT_LISTENER_H
#define BOX_2D_CONTACT_LISTENER_H
#include "Box2DPhysics.h"
#include <Box2D/Box2D.h>
#include <utility>

//#include "Box2DPhysics.h"


class Box2DContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<Box2DPhysics*>(bodyA->GetUserData())->SendCollisionPairAddEvent(bodyA, bodyB);
    }

    void EndContact(b2Contact* contact)
    {
         b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<Box2DPhysics*>(bodyA->GetUserData())->SendCollisionPairRemoveEvent(bodyA, bodyB);
    }
};

#endif