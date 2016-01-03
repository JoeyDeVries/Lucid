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
#ifndef MAP_LOADER_H
#define MAP_LOADER_H
#include "../Components/ActorFactory.h"
#include "../Scene/Scene.h"
//#include "../Scene/SceneNode.h"
#include "../Renderer/Material.h"
// XML libraries here <--
#include "tinyxml2.h"
using namespace tinyxml2;


#include <map>

// could include this functionality in the resource manager, but propagating it to 
// a class to prevent one big bloated class.
class ResourceManager;
class MapLoader
{
private:
	std::map<int, std::shared_ptr<Material>> m_IDToMaterial;

	bool processTileNode(ResourceManager *resources, Scene *scene, XMLElement *tileNode);
	bool processTileData(ResourceManager *resources, Scene *scene, XMLElement *tileData, glm::vec2 pos, glm::vec2 scale, int depth, bool physics);
	bool processGameObject(ResourceManager *resources, Scene *scene, XMLElement *gameObject, glm::vec2 tileScale, float mapScale);
	bool processStaticDefaults(ResourceManager *resources, Scene *scene, XMLElement *map);

    std::string getProperty(XMLElement *object, std::string property);
    std::shared_ptr<Texture2D> getSpecularMapIfExists(ResourceManager *resources, std::string diffusePath);
    std::shared_ptr<Texture2D> getNormalMapIfExists(ResourceManager *resources, std::string diffusePath);
public:
	MapLoader();
	~MapLoader();

	bool LoadMap(ResourceManager *resources, Scene *scene, const char *tmxPath, float levelScale = 1.0f);

};


#endif