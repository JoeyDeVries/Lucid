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
	bool processStaticDefaults(ResourceManager *resources, Scene *scene);
    std::string getProperty(XMLElement *object, std::string property);
public:
	MapLoader();
	~MapLoader();

	bool LoadMap(ResourceManager *resources, Scene *scene, const char *tmxPath, float levelScale = 1.0f);

};


#endif