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

#include "SceneNode.h"

class Scene;

/*
    A SceneNode dedicated to rendering backgrounds. Defines a custom CalculateModel()
    function to ignore positional and rotational transformations. Can be extended to
    include/render 2D parallax maps.
*/
class BackgroundNode : public SceneNode
{
public:
    BackgroundNode(unsigned int ActorID);

    // calculates the model matrix for the background node, ignoring positions and rotations
    virtual void CalculateModel();
    // initializes the background node
    virtual void Initialize(Scene *scene);
    // renders the background node
    virtual void Render(Scene *scene, Renderer *renderer);
};
#endif