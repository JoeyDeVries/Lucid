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

/*
    The root node manages all the different render passes and organizes them
    at a higher level. The root node also sorts all scene nodes based on their
    render data. 
*/
class RootNode : public SceneNode
{
public:
    RootNode();

    // initializes the root node
    virtual void Initialize(Scene *scene);
    // does pre-render operations before rendering its children
    virtual void PreRender(Scene *scene);
    // renders its children w/ different render passes
    virtual void Render(Scene *scene, Renderer *renderer);
    // does post-render operations after rendering its children
    virtual void PostRender(Scene *scene);

    // override children render component to control less general render cases
    virtual void RenderChildren(Scene *scene, Renderer *renderer);
};
#endif