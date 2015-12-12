#include "GUIContainer.h"
#include "../Resources/ResourceManager.h"
#include <algorithm>

GUIContainer::GUIContainer()
{

}

void GUIContainer::AddElement(std::shared_ptr<GUIElement> element)
{
    // check if it doesn't already exist
    for(auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
        if((*it) == element)
            return;
    m_Elements.push_back(element);
}

void GUIContainer::RemoveElement(std::shared_ptr<GUIElement> element)
{
    m_Elements.erase(std::remove(m_Elements.begin(), m_Elements.end(), element), m_Elements.end());
}


bool GUIContainer::IsActive()
{
    return m_IsActive;
}
glm::vec2 GUIContainer::GetPosition()
{
    return m_Position;
}
glm::vec2 GUIContainer::GetScale()
{
    return m_Scale;
}

void GUIContainer::SetActive(bool active)
{
    bool changed = m_IsActive != active;
    m_IsActive = active;
    if(m_IsActive)
        OnActivate();
    else if(changed)
        OnDeactivate();
}
void GUIContainer::SetPosition(glm::vec2 position)
{
    m_Position = position;
}
void GUIContainer::SetScale(glm::vec2 scale)
{
    m_Scale = scale;
}

void GUIContainer::Render(Renderer *renderer, TextRenderer *textRenderer, std::shared_ptr<Camera> camera)
{
    if (m_IsActive)
    {
        // first render background
        RenderBackground(renderer, textRenderer);
        // then children
        std::shared_ptr<Shader> pShader = ResourceManager::GetInstance()->LoadShader("gui", "gui/gui.vs", "gui/gui.frag");
        if (pShader)
        {
            pShader->Use();
            pShader->SetMatrix4("projection", camera->GetProjection());

            for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
            {
                (*it)->Render(renderer, textRenderer, pShader);
            }
        }
    }
}

void GUIContainer::OnActivate()
{

}
void GUIContainer::OnDeactivate()
{

}

void GUIContainer::RenderBackground(Renderer *renderer, TextRenderer *textRenderer)
{
    // by default there is no background, initialize in derivated classes if necessary
}

void GUIContainer::OnKeyPressed(char key)
{

}
void GUIContainer::OnKeyReleased(char key)
{

}
void GUIContainer::OnMouseMove(float x, float y)
{
    if (m_IsActive)
    {
        for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
        {
            (*it)->SetMouseEntered(false);
            if ((*it)->IsMouseInside(x, y))
                (*it)->SetMouseEntered(true);
        }
    }
}
void GUIContainer::OnMouseClick(bool leftButton)
{
    if (m_IsActive)
    {
        std::shared_ptr<GUIButton> pButton;
        for (auto it = m_Elements.begin(); it != m_Elements.end(); ++it)
        {
            if((*it)->GetMouseEntered())
                pButton = std::dynamic_pointer_cast<GUIButton>((*it));
        }
        if (pButton)
        {   // button has been pressed; call generic button pressed function and let class derivations manage logic based on button properties
            ButtonPressed(pButton);
        }
    }
}

void GUIContainer::ButtonPressed(std::shared_ptr<GUIButton> pButton) { }
