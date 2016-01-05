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

/*
    Listens to collision events targeting the owning actor to determine if a
    player touches this component. If a player-touch is registered an 
    'Event_StartLevel' is sent holding the next level's path.
*/
class CompleteCheckComponent : public ActorComponent
{
private:
	std::string m_NextLevelString; // holds the string path to the next level
public:
	CompleteCheckComponent();
	~CompleteCheckComponent();

    // getters
    std::string GetNextLevelString();
    // setters
    void SetNextLevelString(std::string levelPath);

    // initializes the complete check component
	virtual bool VInit();
    // updates the complete check component
	virtual void VUpdate(float delta);

    // listens for post collision events to determine the owning actor-player collision
	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};
#endif