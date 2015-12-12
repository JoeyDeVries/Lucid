#include "TextRenderer.h"
#include "../Resources/ResourceManager.h"

#include <iostream>

TextRenderer::TextRenderer()
{

}

TextRenderer::~TextRenderer()
{

}

bool TextRenderer::Initialize(std::shared_ptr<Font> font)
{
	m_Shader = ResourceManager::GetInstance()->LoadShader("text", "shaders/text.vs", "shaders/text.frag");
    m_Shader->SetInteger("fontAtlas", 0, true);
	m_Font = font;

    // initialize render data
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec4)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


	return true;
}

void TextRenderer::RenderText(std::string text, glm::vec2 position, float userScale, bool inPlace, glm::vec4 color, glm::vec2 centerBox)
{
    float cursor = 0.0f;
    float scale = userScale * m_Font->GetCharSize();
    // calculate left padding to center text if centerBox is supplied
    float centerXPadding = 0.0f;
    float centerYPadding = 0.0f;
    if (centerBox != glm::vec2(1.0f))
    {
        float textWidth = 0.0f;
        for (int i = 0; i < text.size(); ++i) // 1 equal as to not move include the final advance value in the text width
            textWidth += (m_Font->GetCharInfo().at(text[i]).XAdvance / m_Font->GetWidth()) * scale;
        centerXPadding = (centerBox.x - textWidth) / 2.0f;
        centerYPadding = (centerBox.y - (m_Font->GetCharInfo().at(text[0]).Scale.y / m_Font->GetWidth() * scale)) / 2.0f;
    }
    for (char ch : text)
    {
        CharInfo charInfo = m_Font->GetCharInfo().at(ch);
        CharRenderInfo renderInfo;
        for (int i = 0; i < 6; ++i)
        {
            renderInfo.Vertices[i].Position.x = position.x + centerXPadding + charInfo.RenderInfo[i].x * scale + cursor; // position.x
            renderInfo.Vertices[i].Position.y = position.y + centerYPadding + charInfo.RenderInfo[i].y * scale;          // position.y
            renderInfo.Vertices[i].Position.z = charInfo.RenderInfo[i].z; // texcoord.x
            renderInfo.Vertices[i].Position.w = charInfo.RenderInfo[i].w; // texcoord.y
            renderInfo.Vertices[i].Color = color;
        }
        cursor += (charInfo.XAdvance / m_Font->GetWidth()) * scale;
        if(inPlace)
            m_RenderDataInPlace.push_back(renderInfo);
        else
            m_RenderData.push_back(renderInfo);
    }
}

void TextRenderer::Render(glm::mat4 projection, glm::mat4 view)
{
    // first render projected text
    m_Shader->Use();
    m_Font->GetFontAtlas()->Bind(0);
    glBindVertexArray(m_VAO);
        //   fill buffer with render data
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        if (m_RenderData.size() > 0)
        {
            glBufferData(GL_ARRAY_BUFFER, m_RenderData.size() * sizeof(CharRenderInfo), &m_RenderData[0], GL_DYNAMIC_DRAW);
            //   set projection
            m_Shader->SetMatrix4("projection", projection);
            glDrawArrays(GL_TRIANGLES, 0, m_RenderData.size() * 6);
        }
        if (m_RenderDataInPlace.size() > 0)
        {
            // then in-place text
            glBufferData(GL_ARRAY_BUFFER, m_RenderDataInPlace.size() * sizeof(CharRenderInfo), &m_RenderDataInPlace[0], GL_DYNAMIC_DRAW);
            //   set projection (+ camera transform)
            m_Shader->SetMatrix4("projection", projection * view);
            glDrawArrays(GL_TRIANGLES, 0, m_RenderDataInPlace.size() * 6);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // clear current render queue
    m_RenderData.clear();
    m_RenderDataInPlace.clear();
}