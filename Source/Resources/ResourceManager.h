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

#include "MapLoader.h"
#include "Font.h"
#include "../Components/ActorFactory.h"
#include "../Renderer/shader.h"
#include "../Renderer/texture2D.h"
#include "../Renderer/Animation.h"
#include "../Scene/Scene.h"

class ResourceManager
{
private:
    // Cache
    std::map<std::string, std::shared_ptr<Shader>> m_Shaders;
    std::map<std::string, std::shared_ptr<Texture2D>> m_Textures;

    // Singleton pattern, constructor private
    static std::shared_ptr<ResourceManager> m_Instance;
    ResourceManager();
	// Helper function
	std::shared_ptr<Actor> AddSpriteNode(Scene* scene, DEFAULT_ACTOR_TYPES type, std::string name, std::string renderpass, float x, float y, float blockwidth, float blockheight, int depth, std::string shader, std::string diffuse, std::string specular, std::string normal, glm::vec3 color = glm::vec3(1.0));
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
    std::shared_ptr<Shader> LoadShader(std::string name, const char *vertexShaderSource, const char *fragmentShaderSource);
    std::shared_ptr<Shader> GetShader(std::string name);
    std::shared_ptr<Texture2D> LoadTexture(std::string name, const char *textureSource, bool alpha = false);
    std::shared_ptr<Texture2D> GetTexture(std::string name);
	std::shared_ptr<Animation> LoadAnimation(const char *animPath);
	std::shared_ptr<Font>	   LoadFont(const char *fontPath);
	bool LoadLevel(Scene* scene, const char *levelSource, float levelScale = 0.5f);
};

#endif