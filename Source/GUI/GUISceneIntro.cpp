#include "GUISceneIntro.h"
#include "../Application/GameApplication.h"
#include "../Resources/ResourceManager.h"


GUISceneIntro::GUISceneIntro() : m_IntroText(""), m_TimeActive(0.0f), m_Alpha(1.0f), m_TextAlpha(1.0f)
{

}

std::string GUISceneIntro::GetIntroText()
{
    return m_IntroText;
}
void GUISceneIntro::SetIntroText(std::string text)
{
    m_IntroText = text;
}

bool GUISceneIntro::Init()
{

}

void GUISceneIntro::Update(float deltaTime)
{
    if (m_IsActive)
    {
        if (m_TimeActive < 5.0f)
        {
            m_TextAlpha += deltaTime * 0.2f;
        }
        else if (m_TimeActive > 30.0f)
        {
            m_Alpha -= deltaTime * 0.2f;
            m_TextAlpha -= deltaTime * 0.2f;
        }
        else if (m_TimeActive > 35.0f)
        {
            SetActive(false);
        }

        m_TimeActive += deltaTime;
    }
}

void GUISceneIntro::RenderBackground(Renderer *renderer, TextRenderer *textRenderer)
{
    std::shared_ptr<Shader> spriteShader = ResourceManager::GetInstance()->GetShader("sprite");
    spriteShader->Use();
    spriteShader->SetMatrix4("projection", GameApplication::GetInstance()->GetScene()->GetCamera()->GetProjection());
    spriteShader->SetMatrix4("view", glm::mat4());
    spriteShader->SetInteger("EnableLighting", 0);
    spriteShader->SetVector4f("ColorOverride", glm::vec4(1.0f, 1.0f, 1.0f, m_Alpha));
    // 1. first render background
    glm::mat4 model;
    model *= glm::scale(glm::vec3(m_Scale, 1.0f));
    spriteShader->SetMatrix4("model", model);
    m_BackgroundTexture->Bind(0);
    renderer->RenderQuad();
    // 2. then text
    std::vector<std::string> subTexts;
    int charsPerNewline = 50;
    std::string line;
    for(int i = 0; i < m_IntroText.size() ; ++i)
    {
        if (i % charsPerNewline == 0)
        {
            subTexts.push_back(line);
            line = std::string();
        }
        line += m_IntroText[i];
    }
    for(int i = 0; i < subTexts.size(); ++i)
        textRenderer->RenderText(subTexts[i], glm::vec2(100.0f, 50.0f * i), 3.0f, false, glm::vec4(1.0f, 1.0f, 1.0f, m_TextAlpha), glm::vec2(m_Scale.x - 200.0f, 50.0f)); 

    spriteShader->SetInteger("EnableLighting", 1);
}

void GUISceneIntro::OnActivate()
{
    m_Alpha = 1.0f;
    m_TextAlpha = 0.0f;
    m_TimeActive = 0.0f;
}

void GUISceneIntro::OnDeactivate()
{

}



