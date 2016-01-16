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
#ifndef I_PHYSICS_H
#define I_PHYSICS_H

#include <glm/glm.hpp>

#include "../Components/Actor.h"

#include <memory>

/*
    The global interface any physics system should follow.
*/
class IPhysics
{
public:
    // initializes the of physics world
    virtual void Initialize() = 0;
    // updates the physics world (in a fixed timestep to ensure smooth physics)
    virtual void Update() = 0;
    // communicates all physics changes to the render sub-systems for each 'updated' body
    virtual void SyncVisibleScene() = 0;

    // links a sphere collision shape to the supplied actor
    virtual void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool isSensor = false) = 0;
    // links a box collision shape to the supplied actor
    virtual void AddBox(std::shared_ptr<Actor> actor, float density, std::string type = "static", bool fixedRotation = true, bool isSensor = false, float hitBoxScale = 1.0f) = 0;
    // removes an actor from the physics world
    virtual void RemoveActor(unsigned int ActorID) = 0;

    // renders the physics shapes for graphical debugging
    virtual void RenderDiagnostics() = 0;

    // applies a force to a physics entity linked w/ the given actor ID
    virtual void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center) = 0;
    // applies an instant force amount to a physics entity linked w/ the given actor ID
    virtual void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center) = 0;
    // applies a rotational force to physics entity linked w/ the given actor ID
    virtual void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons) = 0;
};
#endif