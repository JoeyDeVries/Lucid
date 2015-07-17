#ifndef SCENE_H
#define SCENE_H

#include "ISceneNode.h"
#include "SceneNode.h"
#include "MatrixStack.h"
#include "Camera.h"

#include <map>
#include <string>
#include <vector>
#include <memory>


typedef std::map<unsigned int, std::shared_ptr<ISceneNode>> SceneActorMap; // relates actor's to SceneNodes

class Scene
{
protected:
    std::shared_ptr<SceneNode> m_Root;
    std::shared_ptr<Camera> m_Camera;
    //std::shared_ptr<IRenderer> m_Renderer; // OpenGL renderer class here!

    MatrixStack m_MatrixStack;
    SceneActorMap m_ActorMap;


public:
    Scene();
    virtual ~Scene();

    void Restore();
    void Update(float deltaTime);
    void Render();

    std::shared_ptr<ISceneNode> FindActor(unsigned int ActorID);
    bool AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child);
    bool RemoveChild(unsigned int ActorID);

    std::shared_ptr<Camera> Camera() { return m_Camera; }

    void PushAndSetMatrix(const glm::mat4 model);
    void PopMatrix();
    const glm::mat4& GetTopMatrix();
};

#endif