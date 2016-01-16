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
#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <vector>
#include <memory>

class LightNode;
class Shader;
class Scene;

/*
    Manages scene nodes of type LightNode and manages the light node's shader 
    interactions. The light manager also makes it possible to only enable
    light nodes of a specific type. Only lights active in the light manager
    are contributed in the final render process.
*/
class LightManager
{
    typedef std::vector<std::shared_ptr<LightNode>> Lights;

    Lights      m_Lights;    // all lights currently active in the light manager
    std::string m_FocusType; // a light type to focus on if set

public:
    LightManager();

    // returns the array of light nodes
    Lights& const GetLights() { return m_Lights; }

    // adds a light node to the list of lights
    void AddLight(std::shared_ptr<LightNode> lightNode);
    // removes a light node from the list of lights
    void RemoveLight(std::shared_ptr<LightNode> lightNode);

    // activates a single type of light source of all the light nodes. Allows for tine-tuned light renders in-between render calls
    void FocusOnLightType(std::string type);
    // removes the focus of a specific light node type
    void RemoveFocus();

    // sends all active (and in range) light nodes to the shader after some slight tweaks (like flickering)
    void UpdateShader(Scene *scene, std::shared_ptr<Shader> shader);
};
#endif