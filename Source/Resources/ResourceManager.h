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

#include "MapLoader.h"
#include "Font.h"

#include <memory>
#include <vector>
#include <string>
#include <map>

class Texture2D;
class Animation;
class Shader;
class Scene;

/*
    General resource manager for loading multiple types of game resources in a general fashion.
*/
class ResourceManager
{
private:
    // cache
    std::map<std::string, std::shared_ptr<Shader>>    m_Shaders;   // holds all previously loaded shaders for easy access
    std::map<std::string, std::shared_ptr<Texture2D>> m_Textures;  // holds all previously loaded textures for easy access
	MapLoader                                         m_MapLoader; // delegated functionality: loads a map/level .tmx file
    
    static std::shared_ptr<ResourceManager> m_Instance; // the specific singleton instance of the class
    ResourceManager();
public:
    // retrieves a single instance of this object
    static std::shared_ptr<ResourceManager> GetInstance()
    {
        if (!m_Instance)
            m_Instance = std::shared_ptr<ResourceManager>(new ResourceManager);
        return m_Instance;
    }
    ~ResourceManager();

    // loads a shader from file with a given name
    std::shared_ptr<Shader>                 LoadShader(std::string name, const char *vertexShaderSource, const char *fragmentShaderSource);
    // retrieves a shader given its name if previously loaded 
    std::shared_ptr<Shader>                 GetShader(std::string name);
    // loads a texture from file with a given name
    std::shared_ptr<Texture2D>              LoadTexture(std::string name, const char *textureSource, bool alpha = false);
    // retrieves a texture given its name if previously loaded 
    std::shared_ptr<Texture2D>              GetTexture(std::string name);
    // loads an animation from file
	std::vector<std::shared_ptr<Animation>> LoadAnimation(const char *animPath);
    // loads a font from file
	std::shared_ptr<Font>	                LoadFont(const char *fontPath);
    // loads a level from file, delegating it to the MapLoader class
	bool                                    LoadLevel(Scene* scene, const char *levelSource, float levelScale = 0.5f);
};
#endif