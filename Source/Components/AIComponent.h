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
#include "ActorComponent.h"
#include <glm/glm.hpp>
#include <memory>

class AIComponent : public ActorComponent
{
private:
    // the radius at which the AI will attack the player
    float m_AttackRadius;
    // its maximum start position to move while walking/attacking (often coordinated with MoveLoopComponent)
    glm::vec2 m_BeginPosition;
    // its maximum end position to move while walking/attack
    glm::vec2 m_EndPosition;
    // denotes whether at this point of time the player is in its vicinity
    bool m_Attacking;
public:
    AIComponent();
    ~AIComponent();

    float GetAttackRadius();
    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    bool IsAttacking();

    void SetAttackRadius(float radius);
    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);
    void SetAttacking(bool attacking);

    bool VInit() override;
    void VUpdate(float deltaTime) override;
};

#endif