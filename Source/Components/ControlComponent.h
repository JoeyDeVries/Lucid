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
#ifndef CONTROL_COMPONENT_H
#define CONTROL_COMPONENT_H

#include "ActorComponent.h"
#include "Event_DestroyActor.h"

/*
    Any actor with a control component attached is controlled via user input. 
*/
class ControlComponent : public ActorComponent
{
private:
    float m_Velocity;  // the maximum velocity of movement
    bool  m_IsJumping; // states whether the actor is currently jumping
	bool  m_OnGround;  // states whether the actor is currently on the ground

    int m_NrGroundCollisionsAdd;    // the number of collision events of the owning actor with the ground
    int m_NrGroundCollisionsRemove; // the number of collision removed events between the actor and the ground
public:
	ControlComponent();
    ~ControlComponent();

    // setter
    void SetVelocity(float velocity);

    // initializes the control component
    bool VInit();
    // updates the control component, reading user input and reacting accordingly
    void VUpdate(float deltaTime);
    // determines ground collisions between actor and environment
	void PostCollisionAdd(std::shared_ptr<IEventData> eventData);
};
#endif