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
#ifndef AI_COMPONENT_H
#define AI_COMPONENT_H

#include <glm/glm.hpp>

#include "ActorComponent.h"

#include <memory>

/*
    Enables the owning Actor to circumvent a (if it exists) MoveLoop component
    to try and attack the player when the player moves within a given target 
    radius. Without a MoveLoop component it would still attack the player if
    within range. Damage done is controlled by a DamageTouch component; if no
    DamageTouchComponent is present the AI deals no damage.
*/
class AIComponent : public ActorComponent
{
private:
    
    float     m_AttackRadius;  // the radius at which the AI will attack the player
    glm::vec2 m_BeginPosition; // its maximum start position to move while walking/attacking (often coordinated with MoveLoopComponent)
    glm::vec2 m_EndPosition;   // its maximum end position to move while walking/attack
    bool      m_Attacking;     // denotes whether at this point of time the player is in its vicinity    
       
    glm::vec2 m_OldPos;            // stores the position at which the AI starts attacking the player
    int       m_RestoreIterations; // keeps track of how many iterations the AI is in its restore position progression
public:
    AIComponent();
    ~AIComponent();

    // getters
    float GetAttackRadius();
    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    bool IsAttacking();
    // setters
    void SetAttackRadius(float radius);
    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);
    void SetAttacking(bool attacking);

    // initializes the AI component
    bool VInit() override;
    // updates the AI component
    void VUpdate(float deltaTime) override;
};
#endif