#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include "../Renderer/shader.h"
#include "LightNode.h"
#include <list>
#include <memory>

class LightManager
{
    typedef std::list<std::shared_ptr<LightNode>> Lights;
    Lights m_Lights;

public:
    LightManager();

    Lights& const GetLights() { return m_Lights; }

    void AddLight(std::shared_ptr<LightNode> lightNode);
    void RemoveLight(std::shared_ptr<LightNode> lightNode);

    void UpdateShader(std::shared_ptr<Shader> shader);
};

#endif