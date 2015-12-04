#include "MapLoader.h"
#include "ResourceManager.h"
#include <iostream>
#include <fstream>

#include "../Application/GameApplication.h"
#include "../Components/Actor.h"
#include "../Scene/SpriteNode.h"
#include "../Scene/LightNode.h"
#include "../Components/StateBlockComponent.h"


MapLoader::MapLoader()
{
}

MapLoader::~MapLoader()
{
}

inline bool file_exists(const std::string& name) 
{
	std::ifstream f(name.c_str());
	bool exists = f.good();
	f.close();
	return exists;
}

bool MapLoader::LoadMap(ResourceManager *resources, Scene *scene, const char * tmxPath, float levelScale)
{
	XMLDocument doc;
	if(doc.LoadFile(tmxPath) != XML_NO_ERROR)
	{
		std::cout << "Failed reading map w/ error name: " << doc.ErrorName() << std::endl;
		return false;
	}
	XMLElement *map = doc.FirstChildElement("map");
	int width = map->IntAttribute("width");
	int height = map->IntAttribute("height");
	float tileWidth = map->IntAttribute("tilewidth") * levelScale;
	float tileHeight = map->IntAttribute("tileheight") * levelScale;

	// process tile entities
	XMLElement *tileset = map->FirstChildElement("tileset");
	XMLElement *tile = tileset->FirstChildElement("tile");
	while (tile)
	{
		if (!processTileNode(resources, scene, tile))
		{
			std::cout << "Failed processing TileNode" << std::endl;
			return false;
		}
		tile = tile->NextSiblingElement();
	}
	// process layers
	XMLElement *layer = map->FirstChildElement("layer");
	int depth = 1; // start from depth 1, 0 is reserved for backgrounds
	while (layer)
	{
		std::string name = std::string(layer->Attribute("name"));
		XMLElement* data = layer->FirstChildElement();
		XMLElement* tile = data->FirstChildElement();
		int rowCounter = 0;
		int columnCounter = 0;
		while (tile)
		{
			// calculate scale, position based on column/rows and depth
			glm::vec2 pos = glm::vec2(columnCounter * tileWidth, rowCounter * tileHeight);
			glm::vec2 scale = glm::vec2(tileWidth, tileHeight);
			if (!processTileData(scene, tile, pos, scale, depth, name == "Physics"))
			{
				std::cout << "Failed processing TileData" << std::endl;
				return false;
			}
			if (++columnCounter == width)
			{
				columnCounter = 0;
				++rowCounter;
			}
			tile = tile->NextSiblingElement();
		}
		layer = layer->NextSiblingElement("layer");
		depth++;
	}
	// then process gameobjects
	XMLElement *gameObjects = map->FirstChildElement("objectgroup");
	if (gameObjects)
	{
		XMLElement *object = gameObjects->FirstChildElement("object");
		while (object)
		{
			if (!processGameObject(resources, scene, object, levelScale))
			{
				std::cout << "Failed processing GameObject" << std::endl;
				return false;
			}

			object = object->NextSiblingElement();
		}
	}

	return true;
}


bool MapLoader::processTileNode(ResourceManager *resources, Scene *scene, XMLElement *tileNode)
{
	int id = tileNode->IntAttribute("id");
	XMLElement *image = tileNode->FirstChildElement("image");
	std::string imageSource = std::string(image->Attribute("source"));
	imageSource = "levels/" + imageSource; // add levels directory so relative paths make sense
	std::shared_ptr<Texture2D> texture = resources->LoadTexture("tex_node_" + std::to_string(id), imageSource.c_str(), true); 
	// configure a material
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());
	material->SetShader(resources->GetShader("sprite"));
	material->SetDiffuse(texture);
	// - check if specular/normal texture exists and if so, add to material; otherwise use default
	std::string imgDir    = imageSource.substr(0, imageSource.find_last_of("\\/"));
	std::string filename  = imageSource.substr(imageSource.find_last_of("\\/") + 1);
	std::string file      = filename.substr(0, filename.find_last_of("."));
	std::string extension = filename.substr(filename.find_last_of(".") + 1);
	std::string specPath  = imgDir + "/" + file + "_specular" + "." + extension;
	std::string normPath  = imgDir + "/" + file + "_normal" + "." + extension;
	if(file_exists(specPath))
		material->SetSpecular(resources->LoadTexture("tex_node_" + std::to_string(id) + "_specular", specPath.c_str()));
	else
		material->SetSpecular(resources->GetTexture("specular")); 
	if (file_exists(normPath))
		material->SetNormal(resources->LoadTexture("tex_node_" + std::to_string(id) + "_normal", normPath.c_str()));
	else
		material->SetNormal(resources->GetTexture("normal"));	  
	// store material properties for re-use by data objects
	m_IDToMaterial[id] = material;
	return true;
}

bool MapLoader::processTileData(Scene *scene, XMLElement *tile, glm::vec2 pos, glm::vec2 scale, int depth, bool physics)
{
	int gid = tile->IntAttribute("gid"); // note: GID are 1-based indices, 0 means empty tile
	if (gid != 0)
	{
		// retrieve material from gid
		std::shared_ptr<Material> material = m_IDToMaterial[gid - 1];
		// create game actor
		std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
		actor->SetPosition(pos);
		actor->SetScale(scale);
		actor->SetDepth(depth);
		// create sprite node
		std::shared_ptr<SpriteNode> node = std::shared_ptr<SpriteNode>(
			new SpriteNode(
				actor->GetID(),
				"sprite",
				"main",
				pos,
				depth,
				scale)
			);
		node->SetMaterial(material);
		scene->AddChild(actor->GetID(), node);
		// if in physics layers, also add collision shapes for each tile
		if (physics)
		{
			GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0f);
		}
	}
	return true;
}

bool MapLoader::processGameObject(ResourceManager *resources, Scene *scene, XMLElement *gameObject, float mapScale)
{
	std::string type = std::string(gameObject->Attribute("type"));
	int x = gameObject->IntAttribute("x");
	int y = gameObject->IntAttribute("y");
	int width = gameObject->IntAttribute("width");
	int height = gameObject->IntAttribute("height");
	glm::vec2 position(x * mapScale, y * mapScale);
	glm::vec2 scale(width * mapScale, height * mapScale);
	// process gameobjects differently based on type
	std::shared_ptr<Material> material = std::shared_ptr<Material>(new Material());
	material->SetShader(resources->GetShader("sprite"));
	if (type == "Spawn") // player spawn
	{
		// define actor
		//std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_PLAYER);
		//actor->SetPosition(position);
		//actor->SetScale(scale);
		//actor->SetDepth(5); // TODO(Joey): seperate render depth from light depth and customize light depth individually
		//GameApplication::GetInstance()->SetImportantActor("player", actor);
		//// set material
		//material->SetDiffuse(resources->LoadTexture("player", "textures/player.png", true));
		//material->SetSpecular(resources->LoadTexture("player_specular", "textures/player_specular.png"));
		//material->SetNormal(resources->LoadTexture("player_normal", "textures/player_normal.png"));
		//// create node
		//std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), "player", "main", position, actor->GetDepth(), scale));
		//node->SetMaterial(material);
		//// create lantern and attach to player
		//std::shared_ptr<Actor> lantern = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_LANTERN);
		//lantern->SetPosition(position + glm::vec2(scale.x * 0.25f, scale.y * 0.5f));
		//lantern->SetDepth(10); 
		//GameApplication::GetInstance()->SetImportantActor("lantern", lantern);
		//std::shared_ptr<LightNode> lanternNode(
		//	new LightNode(lantern->GetID(), "lantern", "LIGHT", position, actor->GetDepth(), scale,
		//		glm::vec3(2.0, 2.0, 2.0), glm::vec3(1.0, 1.0, 1.0), 250.0f)
		//	);
		//// ... we don't need a material/shader as the lantern itself won't be renderable (TODO(Joey): render lantern; could be interesting)
		//node->AddChild(lanternNode);
		//scene->GetLightManager()->AddLight(lanternNode);
		//scene->AddChild(lantern->GetID(), lanternNode);
		//// set physics
		//scene->AddChild(actor->GetID(), node);
		//// point camera at player spawn
		//// ... TODO(Joey)
		//node->SetMaterial(material);

	}
	else if (type == "Finish") // end point
	{
		// define actor
		std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_COMPLETE_CHECK);
		actor->SetPosition(position);
		actor->SetScale(scale);
		actor->SetDepth(4); 
		// set material
		material->SetDiffuse(resources->LoadTexture("flag", "textures/flag.png", true)); 
		material->SetSpecular(resources->LoadTexture("specular", "textures/specular.png"));
		material->SetNormal(resources->LoadTexture("flag_normal", "textures/flag_normal.png"));
		// create node
		std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), "finish", "main", position, actor->GetDepth(), scale));
		node->SetMaterial(material);
		scene->AddChild(actor->GetID(), node);
		// set physics
		GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0, false, true, true, 0.1f);
	}
	else if (type == "StateBlock")
	{
		XMLElement *properties = gameObject->FirstChildElement();
		if (properties)
		{
			XMLElement *property = properties->FirstChildElement("property");
			if (property)
			{
				std::string color = std::string(property->Attribute("value"));
				// define actor
				std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATE_BLOCK);
				actor->SetPosition(position);
				actor->SetScale(scale);
				actor->SetDepth(2);
				// set material
				material->SetDiffuse(resources->LoadTexture("block_color", "textures/stone_color.png", true));
				material->SetSpecular(resources->LoadTexture("block_specular", "textures/stone_specular.png"));
				material->SetNormal(resources->LoadTexture("block_normal", "textures/stone_normal.png"));
				// create node
				std::shared_ptr<SpriteNode> node(new SpriteNode(actor->GetID(), "state_block", "main", position, actor->GetDepth(), scale));
				node->SetMaterial(material);
				scene->AddChild(actor->GetID(), node);
				// set physics
				GameApplication::GetInstance()->GetPhysics()->AddBox(actor, 1.0);
				GameApplication::GetInstance()->GetPhysics()->FindBody(actor->GetID())->GetFixtureList()[0].SetSensor(true);

				// configure state blocks based on color type
				std::weak_ptr<StateBlockComponent> pWeakComponent = actor->GetComponent<StateBlockComponent>("StateBlock");
				std::shared_ptr<StateBlockComponent> pComponent = std::shared_ptr<StateBlockComponent>(pWeakComponent);		
				if (pComponent)
				{
					if (color == "R")
					{
						material->SetColorOverride(glm::vec3(1.0, 0.5, 0.5));
						pComponent->SetBlockColor(LightState::RED);
					}
					else if (color == "G")
					{
						material->SetColorOverride(glm::vec3(0.5, 1.0, 0.5));
						pComponent->SetBlockColor(LightState::GREEN);
					}
					else if (color == "B")
					{
						material->SetColorOverride(glm::vec3(0.5, 0.5, 1.0));
						pComponent->SetBlockColor(LightState::BLUE);
					}
				}
			}
		}
	}
	else if (type == "Light")
	{
		XMLElement *properties = gameObject->FirstChildElement("properties");
		if (properties)
		{
			XMLElement *property = properties->FirstChildElement("property");
			float d_r = property->FloatAttribute("value"); property = property->NextSiblingElement();
			float d_g = property->FloatAttribute("value"); property = property->NextSiblingElement();
			float d_b = property->FloatAttribute("value"); property = property->NextSiblingElement();
			float s_r = property->FloatAttribute("value"); property = property->NextSiblingElement();
			float s_g = property->FloatAttribute("value"); property = property->NextSiblingElement();
			float s_b = property->FloatAttribute("value"); 
			glm::vec3 diffuse(d_r, d_g, d_b);
			glm::vec3 specular(s_r, s_g, s_b);
			// define actor
			std::shared_ptr<Actor> actor = GameApplication::GetInstance()->CreateActor(DEFAULT_ACTOR_TYPES::ACTOR_STATIC);
			actor->SetPosition(position);
			actor->SetScale(scale);
			actor->SetDepth(9);  // TODO(Joey): seperate render depth from light depth and customize light depth individually
			// set material
			material->SetDiffuse(resources->LoadTexture("light-anim", "textures/animations/fire-anim.png", true));
			material->SetSpecular(resources->GetTexture("specular"));
			material->SetNormal(resources->GetTexture("normal"));
			// create node
			std::shared_ptr<LightNode> node(new LightNode(actor->GetID(), "light", "light", position, actor->GetDepth(), scale, diffuse, specular, 250.0f));
			node->SetMaterial(material);
			scene->AddChild(actor->GetID(), node);
			// light has animation, specify animation
			std::shared_ptr<Animation> lightAnim = resources->LoadAnimation("textures/animations/fire-anim.anim");
			if (lightAnim)
			{
				node->SetAnimation(true);
				node->AddAnimation(lightAnim);
			}
		}
	}
	return true;
}