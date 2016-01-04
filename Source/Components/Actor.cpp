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
#include "Actor.h"

Actor::Actor() :  m_Position(0.0), m_Scale(1.0), m_Depth(0)
{

}

Actor::~Actor(void)
{

}

void Actor::Destroy(void)
{
    m_components.clear();
}

void Actor::Update(float deltaTime)
{
    ActorComponents::iterator it = m_components.begin();
    while (it != m_components.end())
        (it++)->second->VUpdate(deltaTime);
}

// NOTE(Joey): See header note
//template<class ComponentType> std::weak_ptr<ComponentType> Actor::GetComponent(std::string type);

void Actor::addComponent(std::shared_ptr<ActorComponent> component)
{
    m_components[component->m_Type] = component;
}

void Actor::setID(ActorID ID)
{
    m_id = ID;
}