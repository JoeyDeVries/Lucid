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

/*
    Allows the owning actor to change its rendering light state and properly informs all
    relevant sub-systems that its light state has changed.
*/
class LightSwitchComponent : public ActorComponent
{
private:
    LightState m_LightState;  // the current state of light the actor has enabled    
    bool       m_Initialized; // disable the light the first update run (light is off by default)
public:
    LightSwitchComponent();
    ~LightSwitchComponent();

    // initializes the light switch component
    bool VInit();
    // updates the light switch component
    void VUpdate(float deltaTime);
};
#endif