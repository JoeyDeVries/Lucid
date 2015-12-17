#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include "../Renderer/shader.h"
#include "LightNode.h"
#include <vector>
#include <memory>

class LightManager
{
    typedef std::vector<std::shared_ptr<LightNode>> Lights;
    Lights m_Lights;
    std::string m_FocusType;

public:
    LightManager();

    Lights& const GetLights() { return m_Lights; }

    void AddLight(std::shared_ptr<LightNode> lightNode);
    void RemoveLight(std::shared_ptr<LightNode> lightNode);

    void FocusOnLightType(std::string type);
    void RemoveFocus();

    void UpdateShader(Scene *scene, std::shared_ptr<Shader> shader);
};

#endif