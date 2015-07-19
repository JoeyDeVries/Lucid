#include "ResourceManager.h"
#include <SOIL.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::shared_ptr<ResourceManager> ResourceManager::m_Instance = std::shared_ptr<ResourceManager>();



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
            return true;
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::LEVEL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }
    return false;
}