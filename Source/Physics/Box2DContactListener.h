/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#ifndef BOX_2D_CONTACT_LISTENER_H
#define BOX_2D_CONTACT_LISTENER_H

#include <Box2D/Box2D.h>

#include "Box2DPhysics.h"

#include <utility>

/*
    The custom Box2DContactListener allows for custom interaction when collisions
    start and end. Whenever a collision occurs we send an event to all the engine's
    sub-systems that a collision occured. Similarly for when a collision ends.
*/
class Box2DContactListener : public b2ContactListener
{
    // fires when a collision between fixtures starts
    void BeginContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<BodyUserData*>(bodyA->GetUserData())->Physics->SendCollisionAddEvent(contact);
    }

    // fires when a collision between fixtures ends
    void EndContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();
        if (bodyA->GetUserData())
            static_cast<BodyUserData*>(bodyA->GetUserData())->Physics->SendCollisionRemoveEvent(contact);
    }
};
#endif