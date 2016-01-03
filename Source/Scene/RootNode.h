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
#ifndef ROOT_NODE_H
#define ROOT_NODE_H

#include "SceneNode.h"


class RootNode : public SceneNode
{
public:
    RootNode();

    virtual void Initialize(Scene *scene);
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene, Renderer *renderer);
    virtual void PostRender(Scene *scene);

    // override children render component to control less general render cases
    virtual void RenderChildren(Scene *scene, Renderer *renderer);
};

#endif