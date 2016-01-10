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
#ifndef BOX_2D_PHYSICS
#define BOX_2D_PHYSICS

#include <Box2D/Box2D.h>

#include "IPhysics.h"

#include "../Components/Actor.h"

#include <memory>
#include <vector>
#include <list>
#include <map>

class Box2DPhysics;
class Box2DContactListener;
class Box2DDebugDrawer;

/*
    Specifies the data each body holds for more precise collision handling.
*/
struct BodyUserData
{
    Box2DPhysics*       Physics;
    DEFAULT_ACTOR_TYPES Type;
};

/*
    Hosts the global physics world and its management functions. An internal state world
    is managed that follows the laws of physics. Movement and collisions are all governed
    by forces and updated movement is communicated to the rendering sub-systems with the
    event communication system. This defines a clear seperation between physics/game-
    logic and rendering.
*/
class Box2DPhysics : public IPhysics
{
    friend Box2DContactListener;
    // physics world
    b2World*              m_World;              // the world containing all physics items
    Box2DContactListener* m_ContactListener;    // custom collision contact listener
    Box2DDebugDrawer*     m_DebugDrawer;        // custom debug drawer for graphically debugging the underlying physics 
    // body-actor relations
    std::map<ActorID, b2Body*> m_ActorIDToBody; // stores the relationships between unique actor IDs and their bodies in the physics world
    std::map<b2Body*, ActorID> m_BodyToActorID; // stores the relationship between bodies in the physics world and their unique actor IDs
    // collisions
	std::list<b2Contact*> m_Collisions;         // collisions occuring each frame
    std::list<ActorID>    m_RemovalQueue;       // actors/bodies scheduled for removal (shouldn't be instantly removed as to not disrupt the physics system)

    // sends a collision add event to all interested game sub-systems
    void SendCollisionAddEvent(b2Contact* contact);
    // sends a collision remove evnet to all interested game sub-systems
    void SendCollisionRemoveEvent(b2Contact * contact);
public:
    Box2DPhysics();
    ~Box2DPhysics();

    // initializes the of physics world
    void Initialize();
    // updates the physics world (in a fixed timestep to ensure smooth physics)
    void Update();
    // communicates all physics changes to the render sub-systems for each 'updated' body
    void SyncVisibleScene();
    // resets the physics system to its default state
    void Reset();

    // returns a body given a unique actor ID
    b2Body* FindBody(ActorID);
    // returns the actor ID linked to a body
    ActorID FindActorID(b2Body*);

    // schedules an actor and its related body for removal
    void RemoveActor(unsigned int ActorID);
    // removes bodies that were scheduled for removal; this is done at a later stage to prevent disrupting the state of the physics world
    void RemoveQueuedItems();
    
    // links a sphere collision shape to the supplied actor
    void AddSphere(float radius, std::shared_ptr<Actor> actor, float density, bool dynamic = false, bool isSensor = false);
    // links a box collision shape to the supplied actor
    void AddBox(std::shared_ptr<Actor> actor, float density, std::string type = "static", bool fixedRotation = true, bool isSensor = false, float hitBoxScale = 1.0f);
    // links a polygon collision shape to the supplied actor
    void AddPolygon(std::shared_ptr<Actor> actor, std::vector<glm::vec2> vertices, float density, bool dynamic = false, bool fixedRotation = true);
    // links a series of pre-defined collision fixtures representing a character given the supplied actor (w/ sensor)
    void AddCharacter(std::shared_ptr<Actor> actor, float density);

    // renders the physics shapes given m_DebugDrawer for graphical debugging
    void RenderDiagnostics();

    // applies a force to a body linked w/ the given actor ID
    void ApplyForce(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    // applies an instant force amount to a body linked w/ the given actor ID
    void ApplyImpulse(unsigned int ActorID, glm::vec2 force, glm::vec2 center);
    // applies a rotational force to a body linked w/ the given actor ID
    void ApplyTorque(unsigned int ActorID, glm::vec2 direction, float newtons);

	// returns the current linear velocity of a body linked to the given actor ID
	glm::vec2 GetLinearVelocity(unsigned int ActorID);
    // sets the linear velocity of a body linked to the given actor ID
    void SetLinearVelocity(unsigned int ActorID, glm::vec2 velocity);
    // returns the total body mass of the body linked to the given actor ID
	float GetBodyMass(unsigned int ActorID);

	// returns whether two bodies are colliding by manually verifying all active collisions
	bool IsBodiesColliding(const b2Body* bodyA, const b2Body* bodyB);	
};
#endif