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
#ifndef LEVEL_COMPLETE_CHECK_H
#define LEVEL_COMPLETE_CHECK_H
#include "ActorComponent.h"
#include "../Communication/IEventData.h"
#include "../Physics/Box2DPhysics.h"
#include "Event_LevelComplete.h"

class CompleteCheckComponent : public ActorComponent
{
private:
	std::string m_NextLevelString;
public:
	CompleteCheckComponent();
	~CompleteCheckComponent();

    std::string GetNextLevelString();
    void SetNextLevelString(std::string levelPath);

	virtual bool VInit();
	virtual void VUpdate(float delta);


	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};


#endif