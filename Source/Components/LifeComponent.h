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
#ifndef LIFE_COMPONENT_H
#define LIFE_COMPONENT_H
#include "ActorComponent.h"

class LifeComponent : public ActorComponent
{
private:
    int m_CurrentHealth;
    int m_MaxHealth;
public:
    LifeComponent();
    ~LifeComponent();

    int GetCurrentHealth();
    int GetMaxHealth();
    void SetCurrentHealth(int health);
    void SetMaxHealth(int health);

    bool VInit();
    void VUpdate(float deltaTime);

    void Heal(int amount);
    void Damage(int amount);
};

#endif