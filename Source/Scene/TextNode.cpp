#include "TextNode.h"
#include "../Application/GameApplication.h"

TextNode::TextNode(unsigned int ActorID, glm::vec2 position, glm::vec2 scale) 
    : SceneNode(ActorID, "text", "TEXT", position, 0, scale, 0.0f), m_Text(""), m_Centered(false)
{

}

std::string TextNode::GetText()
{
    return m_Text;
}
glm::vec4 TextNode::GetColor()
{
    return m_Color;
}
int TextNode::GetFontScale()
{
    return m_FontScale;
}

void TextNode::SetText(std::string text)
{
    m_Text = text;
}
void TextNode::SetColor(glm::vec4 color)
{
    m_Color = color;
}
void TextNode::SetFontScale(int scale)
{
    m_FontScale = scale;
}

void TextNode::Render(Scene *scene, Renderer *renderer)
{
    GameApplication::GetInstance()->GetTextRenderer()->RenderText(m_Text, m_Position, m_FontScale, true, m_Color, m_Scale);
}