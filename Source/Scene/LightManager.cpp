#include "LightManager.h"

LightManager::LightManager()
{

}

void LightManager::AddLight(std::shared_ptr<LightNode> lightNode)
{

}

void LightManager::RemoveLight(std::shared_ptr<LightNode> lightNode)
{

}

void LightManager::UpdateShader(std::shared_ptr<Shader> shader)
{
    // Update all lights that are currently visible to the shader (up to a maximum)
}