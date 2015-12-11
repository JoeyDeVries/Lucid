#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H
#include <glm/glm.hpp>
#include <memory>
#include <string>

class Renderer;
class TextRenderer;
class Shader;

class GUIElement
{
protected:
    std::string m_Name;
    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    glm::vec4 m_ForeColor;
    glm::vec4 m_BackColor;
    glm::mat4 m_Model;
    bool m_MouseEntered;
public:
    GUIElement();

    std::string GetName();
    glm::vec2 GetPosition();
    glm::vec2 GetScale();
    glm::vec4 GetForeColor();
    glm::vec4 GetBackColor();
    bool GetMouseEntered();

    void SetName(std::string name);
    void SetPosition(glm::vec2 position);
    void SetScale(glm::vec2 scale);
    void SetForeColor(glm::vec4 foreColor);
    void SetBackColor(glm::vec4 backColor);
    void SetMouseEntered(bool entered);

    // re-calculates model matrix
    virtual void CalculateModel() = 0;

    // renders the GUIElement, should be overriden in subclasses
    virtual void Render(Renderer *renderer, TextRenderer *textRenderer, std::shared_ptr<Shader> shader) = 0;

    bool IsMouseInside(float x, float y);

    // NOTE(Joey): for GUI elements we don't use the default EventQueue system
    //             as this will incur too much overhead of adding/removing of
    //             events while most GUI elements are inactive most of the time
    // fires when a (relevant) key is pressed
    virtual void OnKeyPressed(char key);
    // fires when a (relevant) key is released
    virtual void OnKeyReleased(char key);
    // fires when the mouse enters this control's dimensions
    virtual void OnMouseEnter();
    // fires when the mouse leaves this control's dimensions
    virtual void OnMouseLeave();
    // fires when the (left) mouse button is clcked
    virtual void OnMouseClick();
};

#endif