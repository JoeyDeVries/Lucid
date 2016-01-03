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

class TextOnTouchComponent : public ActorComponent
{
private:
    std::string m_DisplayText;
    bool m_IsActive;
public:
    TextOnTouchComponent();
    ~TextOnTouchComponent();

    std::string GetDisplayText();
    bool GetIsActive();
    void SetDisplayText(std::string text);

    bool VInit();
    void VUpdate(float deltaTime);

    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
    void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};

#endif