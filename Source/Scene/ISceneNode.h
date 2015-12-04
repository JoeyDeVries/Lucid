#ifndef ISCENE_NODE_H
#define ISCENE_NODE_H

#include "../Renderer/Material.h"

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
    glm::vec2    m_Position;
    glm::vec2    m_Scale;
    int          m_Depth;
    float        m_Rotation;
    float        m_radius; // for frustum visibility occlusion 

    std::shared_ptr<Material>  m_Material;
public:
    virtual ~ISceneNode() { };
    const unsigned int &GetActorID()    const { return m_ActorID; }
    glm::vec2 const    &GetPosition()         { return m_Position; }
    glm::vec2 const    &GetScale()            { return m_Scale; }
    int const          &GetDepth()            { return m_Depth; }
    float const        &GetRotation()         { return m_Rotation; }
    glm::mat4 const    &GetModel()            { return m_Model; }
    const std::string  &GetName()       const { return m_Name; }
    const std::string  &GetRenderPass() const { return m_RenderPass; }

    std::shared_ptr<Material>  GetMaterial()  { return m_Material; }
	
    virtual void CalculateModel() = 0;
    void SetPosition(glm::vec2 position) { m_Position = position; CalculateModel(); }
    void SetScale(glm::vec2 scale)       { m_Scale    = scale;    CalculateModel(); }
    void SetDepth(int depth)             { m_Depth    = depth;    CalculateModel(); }
    void SetRotation(float rotation)     { m_Rotation = rotation; CalculateModel(); }
	void SetName(const std::string& name) { m_Name = name; }
	void SetRenderPass(const std::string& renderPass) { m_RenderPass = renderPass; }
    
    void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    virtual void Initialize(Scene *scene) = 0;
    virtual void Restore(Scene *scene) = 0;
    virtual void Update(Scene *scene, float deltaTime) = 0;
    virtual bool IsVisible(Scene *scene) const = 0;
    virtual void PreRender(Scene *scene) = 0;
    virtual void Render(Scene *scene) = 0;
    virtual void PostRender(Scene *scene) = 0;
    
    virtual bool AddChild(std::shared_ptr<ISceneNode> child) = 0;
    virtual bool RemoveChild(unsigned int actorID) = 0;
    virtual void RenderChildren(Scene *scene) = 0;

	virtual std::vector<std::shared_ptr<ISceneNode>> GetChildren() const = 0;
};

#endif