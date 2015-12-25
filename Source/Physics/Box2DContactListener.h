#ifndef BOX_2D_CONTACT_LISTENER_H
#define BOX_2D_CONTACT_LISTENER_H
#include "Box2DPhysics.h"
#include <Box2D/Box2D.h>
#include <utility>

#include <iostream>

class Box2DContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<BodyUserData*>(bodyA->GetUserData())->Physics->SendCollisionAddEvent(contact);

        std::cout << "collision start: " << static_cast<BodyUserData*>(bodyA->GetUserData())->Type << " : " << static_cast<BodyUserData*>(bodyB->GetUserData())->Type << std::endl;
    }

    void EndContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<BodyUserData*>(bodyA->GetUserData())->Physics->SendCollisionRemoveEvent(contact);
    }
};

#endif