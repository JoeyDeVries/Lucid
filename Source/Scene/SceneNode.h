#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "ISceneNode.h"
#include "../Communication/IEventData.h"

class SceneNode : public ISceneNode
{
    friend class Scene;
protected:
    typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodeList;
    SceneNodeList m_Children;
    SceneNode *m_Parent;
public:
    SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);
    virtual ~SceneNode() { };

    virtual void CalculateModel();

    virtual void Initialize(Scene *scene);
    virtual void Restore(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual bool IsVisible(Scene *scene) const;
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene);
    virtual void PostRender(Scene *scene);

    virtual bool AddChild(std::shared_ptr<ISceneNode> child);
    virtual bool RemoveChild(unsigned int actorID);
    virtual void RenderChildren(Scene *scene);

    void ActorMoved(std::shared_ptr<IEventData> eventData);
};

#endif