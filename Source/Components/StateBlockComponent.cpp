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
#include "StateBlockComponent.h"

#include "ControlComponent.h"

#include "../Application/GameApplication.h"
#include "../Physics/Event_PostCollisionAdd.h"
#include "../Physics/Event_PostCollisionRemove.h"
#include "../Communication/EventManager.h"
#include "../Physics/Box2DPhysics.h"
#include "../Renderer/Material.h"
#include "../Scene/Scene.h"

#include <iostream>

StateBlockComponent::StateBlockComponent() : m_BlockColor(LightState::WHITE), m_LastLightState(LightState::WHITE), m_PlayerContact(false)
{

}

StateBlockComponent::~StateBlockComponent()
{
	// remove callback functions from event queue
	EventListenerDelegate listener1 = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnLightSwitched);
	GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener1, Event_LightStateSwitched::s_EventType);
	EventListenerDelegate listener2 = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnPostCollisionAdd);
	GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener2, Event_PostCollisionAdd::s_EventType);
	EventListenerDelegate listener3 = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnPostCollisionRemove);
	GameApplication::GetInstance()->GetEventManager()->RemoveListener(listener3, Event_PostCollisionRemove::s_EventType);
}

bool StateBlockComponent::VInit()
{
	// register for events
	EventListenerDelegate eventListener = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnLightSwitched);
	GameApplication::GetInstance()->GetEventManager()->AddListener(eventListener, Event_LightStateSwitched::s_EventType);
	EventListenerDelegate listener = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnPostCollisionAdd);
	GameApplication::GetInstance()->GetEventManager()->AddListener(listener, Event_PostCollisionAdd::s_EventType);
	EventListenerDelegate listener2 = fastdelegate::MakeDelegate(this, &StateBlockComponent::OnPostCollisionRemove);
	GameApplication::GetInstance()->GetEventManager()->AddListener(listener2, Event_PostCollisionRemove::s_EventType);
	return true;
}

void StateBlockComponent::VUpdate(float deltaTime)
{

}

void StateBlockComponent::SetBlockColor(LightState blockColor)
{
	m_BlockColor = blockColor;
}

void StateBlockComponent::OnLightSwitched(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_LightStateSwitched> pEvent = std::dynamic_pointer_cast<Event_LightStateSwitched>(eventData);
	if (m_BlockColor != LightState::WHITE && pEvent)
	{
		if (pEvent->GetLightState() == m_BlockColor && !m_PlayerContact)
		{	// Light state corresponds to color of the block, enable collision
			GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID())->GetFixtureList()[0].SetSensor(false);
			// Restore original alpha state
			GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())->GetMaterial()->SetAlpha(1.0f);
		}
		else
		{	// Light state is different than color of this block, disable collision
			GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID())->GetFixtureList()[0].SetSensor(true);
			// Also make them less distinct by making them transparent (if not colored light)
			if (pEvent->GetLightState() != LightState::WHITE)
				GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())->GetMaterial()->SetAlpha(0.15f);
			else // if we have a white light, set transparency back to default
				GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())->GetMaterial()->SetAlpha(1.0f);
		}
		m_LastLightState = pEvent->GetLightState();
	}
}

void StateBlockComponent::OnPostCollisionAdd(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_PostCollisionAdd> data = std::dynamic_pointer_cast<Event_PostCollisionAdd>(eventData);
	if (eventData)
	{
		// get relevant bodies
		const b2Contact* contact = data->GetContact();
		const b2Body* currentBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
		const b2Body* playerBody  = GameApplication::GetInstance()->GetPhysics()->FindBody(GameApplication::GetInstance()->GetImportantActor("player")->GetID());
		// determine if both of them collided
		const b2Body* bodyA = contact->GetFixtureA()->GetBody();
		const b2Body* bodyB = contact->GetFixtureB()->GetBody();
		if (GameApplication::GetInstance()->GetPhysics()->IsBodiesColliding(currentBody, playerBody))
		{	// both relevant bodies collided, now make sure we won't enable physics while player is inside
			m_PlayerContact = true;
		}		
	}
}

void StateBlockComponent::OnPostCollisionRemove(std::shared_ptr<IEventData> eventData)
{
	std::shared_ptr<Event_PostCollisionRemove> data = std::dynamic_pointer_cast<Event_PostCollisionRemove>(eventData);
	if (eventData)
	{
		// get relevant bodies
		const b2Contact* contact = data->GetContact();
		const b2Body* currentBody = GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID());
		const b2Body* playerBody = GameApplication::GetInstance()->GetPhysics()->FindBody(GameApplication::GetInstance()->GetImportantActor("player")->GetID());
		// determine if both of them collided
		const b2Body* bodyA = contact->GetFixtureA()->GetBody();
		const b2Body* bodyB = contact->GetFixtureB()->GetBody();
		//if ((bodyA == currentBody || bodyA == playerBody) && (bodyB == currentBody || bodyB == playerBody))
		if(!GameApplication::GetInstance()->GetPhysics()->IsBodiesColliding(currentBody, playerBody))
		{	// both relevant bodies collided, now make sure we won't enable physics while player is inside
			m_PlayerContact = false;
			if (m_LastLightState == m_BlockColor)
			{
				GameApplication::GetInstance()->GetPhysics()->FindBody(m_Owner->GetID())->GetFixtureList()[0].SetSensor(false);
				GameApplication::GetInstance()->GetScene()->GetSceneNode(m_Owner->GetID())->GetMaterial()->SetAlpha(1.0f);
			}
		}
	}

}

