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
#ifndef BACKGROUND_NODE_H
#define BACKGROUND_NODE_H

#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"

class BackgroundNode : public SceneNode
{
private:
    std::shared_ptr<Camera> m_Camera;
public:
    BackgroundNode(unsigned int ActorID);

    virtual void CalculateModel();
    virtual void Initialize(Scene *scene);
    virtual void Render(Scene *scene, Renderer *renderer);
};


#endif