#include "ISceneNode.h"

class SceneNode : ISceneNode
{
    friend class Scene;
protected:
    typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodeList;
    SceneNodeList m_Children;
    SceneNode *m_Parent;
public:
    SceneNode(unsigned int ActorID, std::string name, std::string renderPass, glm::mat4 model);
    virtual ~SceneNode() { };

    virtual void Restore(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual bool IsVisible(Scene *scene) const;
    virtual void PreRender(Scene *scene);
    virtual void Render(Scene *scene);
    virtual void PostRender(Scene *scene);

    virtual bool AddChild(std::shared_ptr<ISceneNode> child);
    virtual bool RemoveChild(unsigned int actorID);
    virtual void RenderChildren(Scene *scene);
};