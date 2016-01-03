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
#include "LifeComponent.h"

#include "Event_DestroyActor.h"
#include "../Application/GameApplication.h"
#include <algorithm>
#include <memory>

LifeComponent::LifeComponent() : m_CurrentHealth(0), m_MaxHealth(0)
{

}

LifeComponent::~LifeComponent()
{

}

int LifeComponent::GetCurrentHealth()
{
    return m_CurrentHealth;
}
int LifeComponent::GetMaxHealth()
{
    return m_MaxHealth;
}
void LifeComponent::SetCurrentHealth(int amount)
{
    m_CurrentHealth = amount;
}
void LifeComponent::SetMaxHealth(int amount)
{
    m_MaxHealth = amount;
}

bool LifeComponent::VInit()
{
    return true;
}

void LifeComponent::VUpdate(float deltaTime)
{

}

void LifeComponent::Heal(int amount)
{
    m_CurrentHealth = std::min(m_CurrentHealth + amount, m_MaxHealth);
}

void LifeComponent::Damage(int amount)
{
    m_CurrentHealth -= amount;
    if (m_CurrentHealth <= 0)
    {   // Actor died, send event that actor died (use destroy actor event which can be used for multiple things)
        std::shared_ptr<Event_DestroyActor> pEvent(new Event_DestroyActor(m_Owner->GetID()));
        GameApplication::GetInstance()->GetEventManager()->QueueEvent(pEvent);
    }
}