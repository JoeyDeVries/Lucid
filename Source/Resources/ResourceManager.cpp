#include "ResourceManager.h"
#include "../Application/GameApplication.h"
#include "../Renderer/Material.h"
#include "../Scene/SpriteNode.h"
#include "../Scene/BackgroundNode.h"
#include "../Scene/LightNode.h"
#include "../Components/StateBlockComponent.h"

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
    // first check if shader has been loader already, if so; return earlier loaded texture
    auto it = m_Shaders.find(name);
    if (it != m_Shaders.end())
        return it->second;

    std::shared_ptr<Shader> shader(new Shader);
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
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
    shader->Compile(vShaderCode, fShaderCode, std::string(vertexShaderSource).substr(0, std::string(vertexShaderSource).find_last_of('/')));
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
	// first check if texture has been loader already, if so; return earlier loaded texture
	auto it = m_Textures.find(name);
		if (it != m_Textures.end())
			return it->second;

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

bool ResourceManager::LoadLevel(Scene* scene, const char* levelSource, float levelScale)
{
	return m_MapLoader.LoadMap(this, scene, levelSource, levelScale);
}

std::vector<std::shared_ptr<Animation>> ResourceManager::LoadAnimation(const char *animPath)
{
	std::ifstream file(animPath);
	std::string line;
    std::vector<std::shared_ptr<Animation>> animations;
    std::string directory = std::string(animPath).substr(0, std::string(animPath).find_last_of('/'));
    while (std::getline(file, line))
    {   // every 5 lines is a new animation
        // get name
        std::string name = line;
        // get texture(s)
        std::getline(file, line); 
        std::shared_ptr<Texture2D> diffuse = LoadTexture("animation_" + line, (directory + "/" + line).c_str(), true);     
        // get duration
        std::getline(file, line);
        float duration = std::atof(line.c_str());
        // get animation dimensions
        int width = diffuse->Width; int height = diffuse->Height;
        std::getline(file, line);
        int nrCols = std::atoi(line.c_str());
        std::getline(file, line);
        int nrRows = std::atoi(line.c_str());
        // for number of rows*cols, calculate individual frame data
        std::vector<AnimationData> animData;
        for (int y = 0; y < nrRows; ++y)
        {
            for (int x = 0; x < nrCols; ++x)
            {

                AnimationData data;
                data.XOffset = x * (width / nrCols); // TODO(Joey): check for int precision loss
                data.YOffset = y * (height / nrRows);
                data.Width = width / nrCols;
                data.Height = height / nrRows;
                animData.push_back(data);
            }
        }
        std::shared_ptr<Animation> animation(new Animation(animData));
        animation->SetName(name);
        animation->SetDiffuse(diffuse);
        animation->SetSpecular(GetTexture("specular")); // Due to SOIL not always properly loading non-powers-of-two textures (diffuse seems to work for some reason) I leave the specular/normal of animations out for now; 
        animation->SetNormal(GetTexture("normal"));

        animation->SetDuration(duration);
        animation->Normalize(width, height);
        animations.push_back(animation);
    }
    return animations;
}

std::shared_ptr<Font> ResourceManager::LoadFont(const char *fontPath)
{
	std::shared_ptr<Font> font(new Font);
	std::shared_ptr<Texture2D> fontAtlas;
	std::map<char, CharInfo> charInfo;

	std::ifstream file(fontPath);
	std::string line, word;

	// base path
	std::string directory = std::string(fontPath).substr(0, std::string(fontPath).find_last_of('/'));
	
	// header 1
	std::getline(file, line); 
	std::stringstream ss(line);
	ss >> word >> word >> word; // read size=...
	std::string size = word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('='))); 
	int charSize = atoi(size.c_str());

	// header 2
	std::getline(file, line);
	ss.clear();
	ss.str(line);
	ss >> word >> word >> word >> word; // read scaleW=...
	std::string width = word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')));
	int fontWidth = atoi(width.c_str());
	ss >> word;
	width = word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')));
	int fontHeight = atoi(width.c_str());

	// header 3
	std::getline(file, line);
	ss.clear();
	ss.str(line);
	ss >> word >> word >> word; // read file=...
	std::string atlasPath = word.substr(word.find_last_of('=') + 2, (word.size() - word.find_last_of('=') - 3));
	fontAtlas = LoadTexture("fontAtlas", (directory + "/" + atlasPath).c_str(), true);

	// header 4
	std::getline(file, line);
	ss.clear();
	ss.str(line);
	ss >> word >> word; // read count=...
	std::string count = word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')));
	int charCount = atoi(count.c_str());

	for(;charCount > 0; --charCount)
	{
		std::getline(file, line);
		CharInfo chInfo;
		ss.clear();
		ss.str(line);
		// char
		ss >> word >> word; // read id=...
		char id = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// x
		ss >> word; // read x=...
		chInfo.Position.x = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// y
		ss >> word; // read y=...
		chInfo.Position.y = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// width
		ss >> word; // read width=...
		chInfo.Scale.x = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// height
		ss >> word; // read height=...
		chInfo.Scale.y = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// x offset
		ss >> word; // read xoffset=...
		chInfo.Offset.x = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// y offset
		ss >> word; // read yoffset=...
		chInfo.Offset.y = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		// xadvance
		ss >> word; // read xadvance=...
		chInfo.XAdvance = atoi((word.substr(word.find_last_of('=') + 1, (word.size() - word.find_last_of('=')))).c_str());
		charInfo[id] = chInfo;
	}
	font->Load(fontAtlas, charInfo, charSize, fontWidth, fontHeight);
	return font;
}
