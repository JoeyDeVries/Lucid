#ifndef BACKGROUND_NODE_H
#define BACKGROUND_NODE_H

#include "Scene.h"
#include "SceneNode.h"
#include "Camera.h"

class BackgroundNode : public SceneNode
{
private:
    std::shared_ptr<Camera> m_Camera;
public:
    BackgroundNode(unsigned int ActorID);

    virtual void CalculateModel();
    virtual void Initialize(Scene *scene);
    virtual void Render(Scene *scene, Renderer *renderer);
};


#endif