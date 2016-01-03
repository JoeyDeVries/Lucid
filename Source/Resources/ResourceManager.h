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
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "MapLoader.h"
#include "Font.h"
#include "../Components/ActorFactory.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture2D.h"
#include "../Renderer/Animation.h"
#include "../Scene/Scene.h"

/*
    Loads Shaders/Textures from file
    Should also be able to load a level file that further points to multiple resources;
    ResourceManager should then be able to build an entire SceneGraph of renderable SceneNodes
    from this level file, the LoadLevel function gets a Scene pointer to fill.
*/
class ResourceManager
{
private:
    // Cache
    std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
    std::map<std::string, std::shared_ptr<Texture2D>> m_Textures;

    // Singleton pattern, constructor private
    static std::shared_ptr<ResourceManager> m_Instance;
    ResourceManager();
	// Delegation 
	MapLoader m_MapLoader;
public:
    // Retrieves a single instance of this object
    static std::shared_ptr<ResourceManager> GetInstance()
    {
        if (!m_Instance)
            m_Instance = std::shared_ptr<ResourceManager>(new ResourceManager);
        return m_Instance;
    }
    ~ResourceManager();

    // Resource loaders
    std::shared_ptr<Shader>                 LoadShader(std::string name, const char *vertexShaderSource, const char *fragmentShaderSource);
    std::shared_ptr<Shader>                 GetShader(std::string name);
    std::shared_ptr<Texture2D>              LoadTexture(std::string name, const char *textureSource, bool alpha = false);
    std::shared_ptr<Texture2D>              GetTexture(std::string name);
	std::vector<std::shared_ptr<Animation>> LoadAnimation(const char *animPath);
	std::shared_ptr<Font>	                LoadFont(const char *fontPath);
	bool                                    LoadLevel(Scene* scene, const char *levelSource, float levelScale = 0.5f);
};

#endif