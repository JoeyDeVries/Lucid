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

 //   std::vector<std::vector<char>> levelData;
 //   std::ifstream fstream;
 //   // ensures ifstream objects can throw exceptions:
 //   fstream.exceptions(std::ifstream::badbit);
 //   try
 //   {
 //       // Open files
 //       fstream.open(levelSource);      
 //       if (fstream)
 //       {
 //           std::string line;
 //           while (std::getline(fstream, line))
 //           {
 //               std::vector<char> row;
 //               std::istringstream sstream(line);
 //               char tileCode;
 //               while (sstream >> tileCode)
 //                   row.push_back(tileCode);
 //               levelData.push_back(row);
 //           }
 //           fstream.close();
 //       }
 //   }
 //   catch (std::ifstream::failure e)
 //   {
 //       std::cout << "ERROR::LEVEL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
 //       return false;
 //   }

 //   // determine if any level data was loaded; if so, process each tile and build scene
	//if (levelData.size() > 0)
	//{
	//	float blockWidth = GameApplication::GetInstance()->ScreenWidth() / levelData.size();
	//	float blockHeight = GameApplication::GetInstance()->ScreenHeight() / levelData[0].size(); // we can safely index [0] as the size of levelData > 0

	//	// process all rows
	//	for (int y = 0; y < levelData.size(); y++)
	//	{
	//		// process each row individually
	//		for (int x = 0; x < levelData[y].size(); x++)
	//		{
	//			char tileCode = levelData[y][x];
	//			switch (tileCode)
	//			{ // we need braces as we initialize within switch statement: http://stackoverflow.com/questions/5136295/switch-transfer-of-control-bypasses-initialization-of-when-calling-a-function
	//			case '0': // empty
	//			{
	//				break;
	//			}
	//			case '1': // default block
	//			{
	//				AddSpriteNode(scene, DEFAULT_ACTOR_TYPES::ACTOR_STATIC, "block", "MAIN", x, y, blockWidth, blockHeight, 1, "sprite", "block", "block_specular", "block_normal");
	//				//// Create actor
	//				//std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
	//				//actor->SetPosition(glm::vec2(x * blockWidth, y * blockHeight));
	//				//actor->SetScale(glm::vec2(blockWidth, blockHeight));
	//				//actor->SetDepth(1);
	//				//// Then create scenenode
	//				//std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), "block", "MAIN", actor->GetPosition(), actor->GetDepth(), actor->GetScale(), actor->GetRotation()));
	//				//Material material; // configure material (note that the relevant materials should be PRE-loaded)
	//				//// TODO(Joey): Use Data-driven development; configure in data materials for each block + use these to pre-load (or create PreLoadLevel() function) them in GameApplication.
	//				//material.SetShader(GetShader("sprite"));
	//				//material.SetDiffuse(GetTexture("block"));
	//				//material.SetSpecular(GetTexture("block_specular"));
	//				//material.SetNormal(GetTexture("block_normal"));
	//				//node->SetMaterial(material);
	//				//scene->AddChild(actor->GetID(), node);
	//				//// Also define its physics
	//				////GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0, y < 7, false);
	//				//GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0);
	//				break;
	//			}
	//			case '2': // color block RED
	//			{
	//				//GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_ENEMY);
	//				std::shared_ptr<Actor> actor = AddSpriteNode(
	//					scene, DEFAULT_ACTOR_TYPES::ACTOR_STATE_BLOCK, "state_block", "MAIN", 
	//					x, y, blockWidth, blockHeight, 1, 
	//					"sprite", "block_color", "block_specular", "block_normal", glm::vec3(1.0, 0.5, 0.5)
	//				);
	//				// Set color type of block 
	//				std::weak_ptr<StateBlockComponent> pWeakComponent = actor->GetComponent<StateBlockComponent>("StateBlock");
	//				std::shared_ptr<StateBlockComponent> pComponent = std::shared_ptr<StateBlockComponent>(pWeakComponent);
	//				if (pComponent)
	//					pComponent->SetBlockColor(LightState::RED);
	//				GameApplication::GetInstance()->GetPhysics()->FindBody(actor->GetID())->GetFixtureList()[0].SetSensor(true);
	//				break;
	//			}
	//			case '3': // color block GREEN
	//			{
	//				std::shared_ptr<Actor> actor = AddSpriteNode(
	//					scene, DEFAULT_ACTOR_TYPES::ACTOR_STATE_BLOCK, "state_block", "MAIN",
	//					x, y, blockWidth, blockHeight, 1,
	//					"sprite", "block_color", "block_specular", "block_normal", glm::vec3(0.5, 1.0, 0.5)
	//				);
	//				// Set color type of block
	//				std::weak_ptr<StateBlockComponent> pWeakComponent = actor->GetComponent<StateBlockComponent>("StateBlock");
	//				std::shared_ptr<StateBlockComponent> pComponent = std::shared_ptr<StateBlockComponent>(pWeakComponent);
	//				if (pComponent)
	//					pComponent->SetBlockColor(LightState::GREEN);
	//				GameApplication::GetInstance()->GetPhysics()->FindBody(actor->GetID())->GetFixtureList()[0].SetSensor(true);
	//				break;
	//			}
	//			case '4': // color block BLUE
	//			{
	//				std::shared_ptr<Actor> actor = AddSpriteNode(
	//					scene, DEFAULT_ACTOR_TYPES::ACTOR_STATE_BLOCK, "state_block", "MAIN",
	//					x, y, blockWidth, blockHeight, 1,
	//					"sprite", "block_color", "block_specular", "block_normal", glm::vec3(0.5, 0.5, 1.0)
	//					);
	//				// Set color type of block
	//				std::weak_ptr<StateBlockComponent> pWeakComponent = actor->GetComponent<StateBlockComponent>("StateBlock");
	//				std::shared_ptr<StateBlockComponent> pComponent = std::shared_ptr<StateBlockComponent>(pWeakComponent);
	//				if (pComponent)
	//					pComponent->SetBlockColor(LightState::BLUE);
	//				GameApplication::GetInstance()->GetPhysics()->FindBody(actor->GetID())->GetFixtureList()[0].SetSensor(true);
	//				break;
	//			}
	//			case '9': // end-game flag
	//			{
	//				// Create actor
	//				std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_COMPLETE_CHECK);
	//				actor->SetPosition(glm::vec2(x * blockWidth, y * blockHeight));
	//				actor->SetScale(glm::vec2(blockWidth, blockHeight));
	//				actor->SetDepth(1);
	//				//// Then create scenenode
	//				std::shared_ptr<SpriteNode> node(
	//					new SpriteNode(actor->GetID(), "complete", "MAIN", actor->GetPosition(), actor->GetDepth(), actor->GetScale(), actor->GetRotation())
	//				);
	//				std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());								
	//				material->SetShader(GetShader("sprite"));
	//				material->SetDiffuse(GetTexture("flag"));
	//				material->SetSpecular(GetTexture("specular"));
	//				material->SetNormal(GetTexture("flag_normal"));
	//				node->SetMaterial(material);
	//				scene->AddChild(actor->GetID(), node);
	//				GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0, false, true, true, 0.1f);
	//				break;
	//			}
	//			default:
	//				break;
	//			}

	//		}
	//	}
		// few lights TODO(Joey): build lights in the level data
		//std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
		//actor->SetPosition(glm::vec2(170.0f, 180.0f));
		//actor->SetScale(glm::vec2(50, 50));
		//actor->SetDepth(9);
		//std::shared_ptr<LightNode> node(
		//	new LightNode(actor->GetID(), "light", "LIGHT", actor->GetPosition(), actor->GetDepth(), actor->GetScale(),
		//		glm::vec3(1.3, 2.0, 2.0), glm::vec3(0.7), 250.0f)
		//	);
		//std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());
		//material->SetShader(GetShader("sprite"));
		//material->SetDiffuse(GetTexture("light"));
		//material->SetSpecular(GetTexture("specular"));
		//material->SetNormal(GetTexture("normal"));
		//node->SetMaterial(material);
		//scene->AddChild(actor->GetID(), node);
		// second light
		//std::shared_ptr<Actor> actor2 = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
		//actor2->SetPosition(glm::vec2(400.0f, 300.0f));
		//actor2->SetScale(glm::vec2(50, 50));
		//actor2->SetDepth(9);
		//std::shared_ptr<LightNode> node2(
		//	new LightNode(actor2->GetID(), "light", "LIGHT", actor2->GetPosition(), actor2->GetDepth(), actor2->GetScale(),
		//		glm::vec3(1.8, 1.3, 1.3), glm::vec3(0.7), 250.0f)
		//	);
		//std::shared_ptr<Material> material2 = std::shared_ptr<Material>(new Material());
		//material2->SetShader(GetShader("sprite"));
		//material2->SetDiffuse(GetTexture("light-anim"));
		//material2->SetSpecular(GetTexture("specular"));
		//material2->SetNormal(GetTexture("normal"));
		//node2->SetMaterial(material2);
		//// load light animation
		//std::shared_ptr<Animation> lightAnim = LoadAnimation("textures/animations/fire-anim.anim");
		//if (lightAnim)
		//{
		//	node2->SetAnimation(true);
		//	node2->AddAnimation(lightAnim);
		//}
		//scene->AddChild(actor2->GetID(), node2);
	//}

    //return false;
}

// helper function
std::shared_ptr<Actor> ResourceManager::AddSpriteNode(Scene* scene, DEFAULT_ACTOR_TYPES type, std::string name, std::string renderpass, float x, float y, float blockwidth, float blockheight, int depth, std::string shader, std::string diffuse, std::string specular, std::string normal, glm::vec3 color)
{
	// Create actor
	std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(type);
	actor->SetPosition(glm::vec2(x * blockwidth, y * blockheight));
	actor->SetScale(glm::vec2(blockwidth, blockheight));
	actor->SetDepth(depth);
	// Then create scenenode
	std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), name, renderpass, actor->GetPosition(), actor->GetDepth(), actor->GetScale(), actor->GetRotation()));
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material()); // configure material (note that the relevant materials should be PRE-loaded)
					   // TODO(Joey): Use Data-driven development; configure in data materials for each block + use these to pre-load (or create PreLoadLevel() function) them in GameApplication.
	material->SetShader(GetShader(shader));
	material->SetDiffuse(GetTexture(diffuse));
	material->SetSpecular(GetTexture(specular));
	material->SetNormal(GetTexture(normal));
	material->SetColorOverride(color);
	node->SetMaterial(material);
	scene->AddChild(actor->GetID(), node);
	// Also define its physics
	//GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0, y < 7, false);
	GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0);
	return actor;
}

std::shared_ptr<Animation> ResourceManager::LoadAnimation(const char *animPath)
{
	std::ifstream file(animPath);
	std::string line;
	std::vector<AnimationData> animData;
	// first obtain header
	std::string header;
	if (std::getline(file, header))
	{
		std::stringstream sstream(header);
		float duration;
		sstream >> duration;		
		// then read in each animation frame
		while (std::getline(file, line))
		{
			sstream.str(line);			
			sstream.clear();
			AnimationData data;
			sstream >> data.XOffset;
			sstream >> data.YOffset;
			sstream >> data.Width;
			sstream >> data.Height;
			animData.push_back(data);
		}
		std::shared_ptr<Animation> animation = std::shared_ptr<Animation>(new Animation(animData));
		animation->SetDuration(duration);
		animation->Normalize();
		return animation;
	}
	return std::shared_ptr<Animation>();
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
