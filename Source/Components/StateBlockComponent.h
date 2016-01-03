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
#ifndef STATE_BLOCK_COMPONENT_H
#define STATE_BLOCK_COMPONENT_H
#include "ActorComponent.h"
#include "Event_LightStateSwitched.h"

class StateBlockComponent : public ActorComponent
{
private:
	LightState m_BlockColor;
	LightState m_LastLightState;
	bool m_PlayerContact;

public:
	StateBlockComponent();
	~StateBlockComponent();

	virtual bool VInit();
	virtual void VUpdate(float delta);

	void SetBlockColor(LightState blockColor);

	void OnLightSwitched(std::shared_ptr<IEventData> eventData);
	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
	void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};


#endif