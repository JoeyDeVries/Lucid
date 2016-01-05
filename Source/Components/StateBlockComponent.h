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
#ifndef STATE_BLOCK_COMPONENT_H
#define STATE_BLOCK_COMPONENT_H

#include "ActorComponent.h"
#include "Event_LightStateSwitched.h"

/*
    Allows an actor to respond to different states of (colored) light. As soon
    as light state is switched (by the player's lantern or any other actor with 
    similar capabilities) the actor's collision is enabled or disabled based on
    whether the state block component's m_BlockColor corresponds to the active
    new light state.
*/
class StateBlockComponent : public ActorComponent
{
private:
	LightState m_BlockColor;     // specifies at which light state this block enables its collision
	LightState m_LastLightState; // stores the last sent light state to make sure we don't enable collisions when the player is inside the shape
	bool       m_PlayerContact;  // stores whether the player is currently in contact with the block; then we don't enable collisions

public:
	StateBlockComponent();
	~StateBlockComponent();

    // initializes the state block component
	virtual bool VInit();
    // updates the state block component
	virtual void VUpdate(float delta);

    // sets the color state the actor reacts to
	void SetBlockColor(LightState blockColor);

    // fires as soon as the light state's switched to update collision shapes if possible
	void OnLightSwitched(std::shared_ptr<IEventData> eventData);
    // determines whether the player is inside the actor's collision shape 
	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
    // determines whether the player left the actor's collision shape so actor collisions can be (re-)enabled
	void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};
#endif