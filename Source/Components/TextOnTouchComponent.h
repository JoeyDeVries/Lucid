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
#ifndef TEXT_ON_TOUCH_COMPONENT_H
#define TEXT_ON_TOUCH_COMPONENT_H

#include "ActorComponent.h"

#include "../Communication/IEventData.h"

#include <string>

/*
    Enables the owning actor to display a string of text as soon as the player
    touches the collision shape of the owning actor.
*/
class TextOnTouchComponent : public ActorComponent
{
private:
    std::string m_DisplayText; // the text to display
    bool        m_IsActive;    // whether the text should currently display e.g. the component is active
public:
    TextOnTouchComponent();
    ~TextOnTouchComponent();

    // getters
    std::string GetDisplayText();
    bool GetIsActive();
    // setters
    void SetDisplayText(std::string text);

    // initializes the text on touch component
    bool VInit();
    // updates the text on touch component
    void VUpdate(float deltaTime);

    // determines whether the player is currently inside the owning actor's collision shape
    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
    // checks whether the player left the owning actor's collision shape
    void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};
#endif