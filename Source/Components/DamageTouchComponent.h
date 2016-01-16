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
#ifndef DAMAGE_TOUCH_COMPONENT_H
#define DAMAGE_TOUCH_COMPONENT_H

#include "ActorComponent.h"

#include "../Communication/IEventData.h"

#include <memory>

/*
    Any actor that has the damage touch component will do damage to colliding actors
    as soon as the collision shape collides with another actor's collision shape.
    The damage can be specified as a very large amount to generate a so called 
    'DeathTouch' component, instantly killing the respective actor on touch.
    Note that damage can only be dealt if the other actor(s) own a life component.
*/
class DamageTouchComponent : public ActorComponent
{
private:
    int   m_DamageAmount;        // the amount of damage to deal to other actor(s) on touch
    float m_TimeSinceLastDamage; // a timer to only deal damage after certain intervals to prevent successive damgae hits
public:
    DamageTouchComponent();
    ~DamageTouchComponent();

    // getters
    int GetDamageAmount();
    // setters
    void SetDamageAmount(int amount);

    // initializes the damage touch component
    bool VInit();
    // updates the damage touch component
    void VUpdate(float deltaTime);

    // determines collision between other actors and deals damage if other actor has a life component
    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};
#endif