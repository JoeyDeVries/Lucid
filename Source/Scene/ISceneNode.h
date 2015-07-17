#ifndef ISCENE_NODE_H
#define ISCENE_NODE_H

#include <string>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>



class Scene;

class ISceneNode
{
protected:
    unsigned int m_ActorID;
    std::string  m_Name;
    std::string  m_RenderPass;
    glm::mat4    m_Model;
    glm::vec3    m_Position;
    float        m_Scale;
    float        m_radius; // for frustum visibility occlusion 
    // material m_Material
public:
    virtual ~ISceneNode() { };
    const unsigned int &ActorID() const { return m_ActorID; }
    glm::vec3 const    &Position() const { return m_Position; }
    float const        &Scale() const { return m_Scale; }
    glm::mat4 const    &Model() const { return m_Model; }
    const std::string  &Name() const { return m_Name; }
    const std::string  &RenderPass() const { return m_RenderPass; }
    // TODO: getter Material
    void SetTransform(glm::mat4 &model) { m_Model = model; }

    virtual void Restore(Scene *scene) = 0;
    virtual void Update(Scene *scene, float deltaTime) = 0;
    virtual bool IsVisible(Scene *scene) const = 0;
    virtual void PreRender(Scene *scene) = 0;
    virtual void Render(Scene *scene) = 0;
    virtual void PostRender(Scene *scene) = 0;
    
    virtual bool AddChild(std::shared_ptr<ISceneNode> child) = 0;
    virtual bool RemoveChild(unsigned int actorID) = 0;
    virtual void RenderChildren(Scene *scene) = 0;
};

#endif