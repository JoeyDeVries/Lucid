#ifndef TEXT_NODE_H
#define TEXT_NODE_H
#include "SceneNode.h"
#include <glm/glm.hpp>

class Scene;
class Renderer;

class TextNode : public SceneNode
{
private:
    std::string m_Text;
    glm::vec4   m_Color;
    int         m_FontScale;
    bool        m_Centered;
public:
    TextNode(unsigned int ActorID, glm::vec2 position, glm::vec2 scale = glm::vec2(1.0f));

    std::string GetText();
    glm::vec4   GetColor();
    int         GetFontScale();

    void SetText(std::string text);
    void SetColor(glm::vec4 color);
    void SetFontScale(int scale);


    void Render(Scene *scene, Renderer *renderer) override;
};



#endif