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
#ifndef LIGHT_SWITCH_COMPONENT_H
#define LIGHT_SWITCH_COMPONENT_H
#include "ActorComponent.h"
#include "Event_LightStateSwitched.h"



class LightSwitchComponent : public ActorComponent
{
private:
	LightState m_LightState;
    // disables the light the first update run (light is off by default)
    bool m_Initialized; 
public:
	LightSwitchComponent();
	~LightSwitchComponent();

	bool VInit();
	void VUpdate(float deltaTime);	
};

#endif