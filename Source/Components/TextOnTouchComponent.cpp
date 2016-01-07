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
#include "TextOnTouchComponent.h"

#include "../Physics/Event_PostCollisionRemove.h"
#include "../Physics/Event_PostCollisionAdd.h"
#include "../Application/GameApplication.h"
#include "../Communication/EventManager.h"
#include "../Renderer/TextRenderer.h"
#include "../Physics/Box2DPhysics.h"

TextOnTouchComponent::TextOnTouchComponent() : m_DisplayText(""), m_IsActive(false)
{

}

TextOnTouchComponent::~TextOnTouchComponent()
{
    // remove event listeners from manager
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &TextOnTouchComponent::OnPostCollisionAdd);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_PostCollisionAdd::s_EventType);
    listener = fastdelegate::MakeDelegate(this, &TextOnTouchComponent::OnPostCollisionRemove);
    GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener, Event_PostCollisionRemove::s_EventType);
}


std::string TextOnTouchComponent::GetDisplayText()
{
    return m_DisplayText;
}
bool TextOnTouchComponent::GetIsActive()
{
    return m_IsActive;
}

void TextOnTouchComponent::SetDisplayText(std::string text)
{
    m_DisplayText = text;
}

bool TextOnTouchComponent::VInit()
{
    // register event listeners
    EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &TextOnTouchComponent::OnPostCollisionAdd);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_PostCollisionAdd::s_EventType);
    listener = fastdelegate::MakeDelegate(this, &TextOnTouchComponent::OnPostCollisionRemove);
    GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_PostCollisionRemove::s_EventType);
    return true;
}

void TextOnTouchComponent::VUpdate(float deltaTime)
{
    // not completely MVC, but seeing as text gets enqueued (much more efficient) for later rendering this is ok
    if (m_IsActive)
    {
        glm::vec2 centerBox = glm::vec2(300.0f, 50.0f);
        glm::vec2 position = m_Owner->GetPosition() - glm::vec2(centerBox.x * 0.5f - m_Owner->GetScale().x * 0.5f, m_Owner->GetScale().y);
        GameApplication::GetInstance()->GetTextRenderer()->RenderText(m_DisplayText, position, 1.8f, true, glm::vec4(0.75f), centerBox);
    }
}

void TextOnTouchComponent::OnPostCollisionAdd(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_PostCollisionAdd> pEvent = std::dynamic_pointer_cast<Event_PostCollisionAdd>(eventData);
    if (pEvent)
    {
        const b2Body *currBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
        const b2Body *playerBody = GameApplication::GetInstance()->GetPhysics()->FindBody(GameApplication::GetInstance()->GetImportantActor("player")->GetID());
        if(GameApplication::GetInstance()->GetPhysics()->IsBodiesColliding(currBody, playerBody))
            m_IsActive = true;
    }
}

void TextOnTouchComponent::OnPostCollisionRemove(std::shared_ptr<IEventData> eventData)
{
    std::shared_ptr<Event_PostCollisionRemove> pEvent = std::dynamic_pointer_cast<Event_PostCollisionRemove>(eventData);
    if (pEvent)
    {
        const b2Body *currBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
        const b2Body *playerBody = GameApplication::GetInstance()->GetPhysics()->FindBody(GameApplication::GetInstance()->GetImportantActor("player")->GetID());
        if (!GameApplication::GetInstance()->GetPhysics()->IsBodiesColliding(currBody, playerBody))
            m_IsActive = false;
    }
}
