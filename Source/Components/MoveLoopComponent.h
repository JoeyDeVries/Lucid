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
#ifndef MOVE_LOOP_COMPONENT_H
#define MOVE_LOOP_COMPONENT_H

#include <glm/glm.hpp>

#include "ActorComponent.h"

/*
    Enables the actor to continously move in a pre-defined pattern. This is useful
    for moving platforms and/or enemies. The move loop component is only defined by
    a 2D begin and end position allowing multiple movement cycles to be defined in 
    the map editor. For instance, a vertical movement box creates vertical 
    platforms; this also allows for diagonal platforms.
*/
class MoveLoopComponent : public ActorComponent
{
private:
    glm::vec2 m_BeginPosition;   // the begin position of its movement cycle
    glm::vec2 m_EndPosition;     // the end position of its movement cycle
    float     m_Speed;           // the speed at which the actor moves
    float     m_CurrentPosition; // the current position of the actor
    bool      m_Forward;         // whether it is currently moving from begin to end
    bool      m_Paused;          // whether the looping movement is put on halt
public:
    MoveLoopComponent();
    ~MoveLoopComponent();

    // getters
    glm::vec2 GetBeginPosition();
    glm::vec2 GetEndPosition();
    float GetSpeed();
    // setters
    void SetBeginPosition(glm::vec2 position);
    void SetEndPosition(glm::vec2 position);
    void SetSpeed(float speed);

    // initializes the move loop component
    bool VInit();
    // updates the move loop component, determining its current position in the cycle if not halted
    void VUpdate(float deltaTime);

    // allows other controls/sub-systems to temporarily halt the movement cycle for injection of custom movement logic
    void Pause();
    // allows other controls/sub-systems to resume the movement cycle 
    void Resume();
};
#endif