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
// any actor that has this control component attached is controlled via user input
// a similar component can be developed for AI control


#include "ActorComponent.h"
#include "Event_DestroyActor.h"


class ControlComponent : public ActorComponent
{
private:
    float m_Velocity;
    bool  m_IsJumping;
public:
	bool  m_OnGround;
	ControlComponent();
    ~ControlComponent();
    bool VInit();
    void VUpdate(float deltaTime);
	void PostCollisionAdd(std::shared_ptr<IEventData> eventData);
	void PostCollisionRemove(std::shared_ptr<IEventData> eventData);

    void SetVelocity(float velocity);
};

#endif