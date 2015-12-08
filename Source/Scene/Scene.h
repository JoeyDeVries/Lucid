#ifndef SCENE_H
#define SCENE_H

#include "ISceneNode.h"
#include "RootNode.h"
#include "MatrixStack.h"
#include "Camera.h"
#include "LightManager.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/TextRenderer.h"
#include <map>
#include <string>
#include <vector>
#include <memory>


typedef std::map<unsigned int, std::shared_ptr<ISceneNode>> SceneActorMap; // relates actor's to SceneNodes

class Scene
{
protected:
    std::shared_ptr<RootNode> m_Root;
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<Renderer> m_Renderer; // OpenGL renderer class here!
	std::shared_ptr<LightManager> m_LightManager;

    MatrixStack m_MatrixStack;
    SceneActorMap m_ActorMap;

	int m_SceneWidth, m_SceneHeight;

public:
    Scene();
    virtual ~Scene();

    void Initialize();
    void Restore();
    void Update(float deltaTime);
    void Render();

    std::shared_ptr<ISceneNode> GetSceneNode(unsigned int ActorID);
    bool AddChild(unsigned int ActorID, std::shared_ptr<ISceneNode> child);
    bool RemoveChild(unsigned int ActorID);

    std::shared_ptr<Renderer>     const GetRenderer()       { return m_Renderer; }
    std::shared_ptr<Camera>       const GetCamera()         { return m_Camera; }
    std::shared_ptr<LightManager> const GetLightManager()   { return m_LightManager; }
	std::shared_ptr<RootNode>	  const GetRootNode()		{ return m_Root; }

	int  GetSceneWidth()  const	   { return m_SceneWidth; }
	int  GetSceneHeight() const    { return m_SceneHeight; }
	void SetSceneWidth(int width)  { m_SceneWidth = width; }
	void SetSceneHeight(int height){ m_SceneHeight = height; }

    void PushAndSetMatrix(const glm::mat4 model);
    void PopMatrix();
    const glm::mat4& GetTopMatrix();
};

#endif