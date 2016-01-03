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

class DamageTouchComponent : public ActorComponent
{
private:
    int m_DamageAmount;
    float m_TimeSinceLastDamage;
public:
    DamageTouchComponent();
    ~DamageTouchComponent();

    int GetDamageAmount();
    void SetDamageAmount(int amount);

    bool VInit();
    void VUpdate(float deltaTime);

    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};

#endif