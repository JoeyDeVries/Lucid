#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H
#include "SceneNode.h"
#include "Scene.h"
#include <memory>

class SpriteNode : public SceneNode
{
public:
    SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    virtual void Initialize(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual void Render(Scene *scene);

    void ActorMoved(std::shared_ptr<IEventData> eventData);
};

#endif