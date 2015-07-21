#include "ResourceManager.h"
#include "../Application/GameApplication.h"
#include "../Renderer/Material.h"

#include <SOIL.h>

#include <iostream>
#include <fstream>
#include <sstream>

std::shared_ptr<ResourceManager> ResourceManager::m_Instance = std::shared_ptr<ResourceManager>();



ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

std::shared_ptr<Shader> ResourceManager::LoadShader(std::string name, const char* vertexShaderSource, const char* fragmentShaderSource)
{
    std::shared_ptr<Shader> shader(new Shader);
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexShaderSource);
        fShaderFile.open(fragmentShaderSource);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();

    // 2. Now create and store shader object from source code
    shader->Compile(vShaderCode, fShaderCode);
    shader->Name = name;
    m_Shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ResourceManager::GetShader(std::string name)
{
    auto it = m_Shaders.find(name);
    if(it != m_Shaders.end())
        return it->second;
    else
        return std::shared_ptr<Shader>();
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture(std::string name, const char* textureSource, bool alpha)
{
    std::shared_ptr<Texture2D> texture(new Texture2D);
    // Generate texture from file
    if (alpha)
    {
        texture->InternalFormat = GL_RGBA;
        texture->ImageFormat    = GL_RGBA;
    }
    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(textureSource, &width, &height, 0, texture->ImageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    // Now generate texture
    texture->Generate(width, height, image);
    // And finally free image data
    SOIL_free_image_data(image);

    // Store and return
    m_Textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture2D> ResourceManager::GetTexture(std::string name)
{
    auto it = m_Textures.find(name);
    if (it != m_Textures.end())
        return it->second;
    else
        return std::shared_ptr<Texture2D>();
}

bool ResourceManager::LoadLevel(Scene* scene, const char* levelSource)
{
    std::vector<std::vector<char>> levelData;
    std::ifstream fstream;
    // ensures ifstream objects can throw exceptions:
    fstream.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        fstream.open(levelSource);      
        if (fstream)
        {
            std::string line;
            while (std::getline(fstream, line))
            {
                std::vector<char> row;
                std::istringstream sstream(line);
                char tileCode;
                while (sstream >> tileCode)
                    row.push_back(tileCode);
                levelData.push_back(row);
            }
            fstream.close();
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::LEVEL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }

    // determine if any level data was loaded; if so, process each tile and build scene
    if (levelData.size() > 0)
    {
        float blockWidth = GameApplication::GetInstance()->ScreenWidth() / levelData.size();
        float blockHeight = GameApplication::GetInstance()->ScreenHeight() / levelData[0].size(); // we can safely index [0] as the size of levelData > 0

        // process all rows
        for (int y = 0; y < levelData.size(); y++)
        {
            // process each row individually
            for (int x = 0; x < levelData[y].size(); x++)
            {
                char tileCode = levelData[y][x];
                switch (tileCode)
                { // we need braces as we initialize within switch statement: http://stackoverflow.com/questions/5136295/switch-transfer-of-control-bypasses-initialization-of-when-calling-a-function
                case '0': // empty
                {
                    break;
                }
                case '1': // default block
                {
                    // Create actor
                    std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
                    actor->Position() = glm::vec2(x * blockWidth, y * blockHeight);
                    actor->Scale() = glm::vec2(blockWidth, blockHeight);
                    // Then create scenenode
                    std::shared_ptr<SceneNode> node(new SceneNode(actor->GetID(), "block", "MAIN", actor->Position(), actor->Scale(), actor->Rotation()));
                    Material material; // configure material (note that the relevant materials should be PRE-loaded)
                    // TODO: Use Data-driven development; configure in data materials for each block + use these to pre-load (or create PreLoadLevel() function) them in GameApplication.
                    material.SetShader(GetShader("sprite"));
                    material.SetDiffuse(GetTexture("block"));
                    node->SetMaterial(material);
                    scene->AddChild(actor->GetID(), node);
                    break;
                }
                case '2': // color block
                {
                    GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_ENEMY);
                    break;
                }
                case '3': // collectible
                {
                    break;
                }
                default:
                    break;
                }
            }
        }
        // next initialize scene (e.g. set default projection matrices for each shader)
        scene->Initialize(); 
    }

    return false;
}