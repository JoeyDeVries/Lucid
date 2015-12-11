#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "GUIElement.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/TextRenderer.h"
#include "../Renderer/shader.h"

#include <string>

class GUIButton : public GUIElement
{
private:
    std::string m_Text;
    glm::vec4   m_HoverColor;
public:
    GUIButton();

    std::string GetText();
    glm::vec4 GetHoverColor();

    void SetText(std::string text);
    void SetHoverColor(glm::vec4 color);

    void CalculateModel();
    void Render(std::shared_ptr<Renderer> renderer, TextRenderer *textRenderer, std::shared_ptr<Shader> shader);
};

#endif