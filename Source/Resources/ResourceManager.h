#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
/*
    Loads Shaders/Textures from file
    Should also be able to load a level file that further points to multiple resources;
    ResourceManager should then be able to build an entire SceneGraph of renderable SceneNodes
    from this level file, the LoadLevel function gets a Scene pointer to fill.
*/
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include "../Renderer/shader.h"
#include "../Renderer/texture2D.h"
#include "../Scene//Scene.h"

class ResourceManager
{
    // Cache
    std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
    std::map<std::string, std::shared_ptr<Texture2D>> m_Shaders;

    // Singleton pattern, constructor private
    static std::shared_ptr<ResourceManager> m_Instance;
    ResourceManager() { }
public:
    // Retrieves a single instance of this object
    static std::shared_ptr<ResourceManager> GetInstance()
    {
        if (!m_Instance)
            m_Instance = std::shared_ptr<ResourceManager>(new ResourceManager);
        return m_Instance;
    }

    // Resource loaders
    std::shared_ptr<Shader> LoadShader(std::string name, const char* vertexShaderSource, const char* fragmentShaderSource, const char *geometrySource = nullptr);
    std::shared_ptr<Shader> GetShader(std::string name);
    std::shared_ptr<Texture2D> LoadTexture(std::string name, const char* textureSource);
    std::shared_ptr<Texture2D> GetTexture(std::string name);
    bool LoadLevel(Scene* scene, const char* levelSource);
};

#endif