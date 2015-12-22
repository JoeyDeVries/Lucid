#ifndef GUI_SCENE_INTRO_H
#define GUI_SCENE_INTRO_H
#include "GUIContainer.h"

class GUISceneIntro : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture;
    std::string m_IntroText;
    float m_Alpha;
    float m_TextAlpha;
    float m_TimeActive;
public:
    GUISceneIntro();

    std::string GetIntroText();
    void SetIntroText(std::string text);

    bool Init() override;
    void Update(float deltaTime) override;
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override; 

    void OnActivate() override;
    void OnDeactivate() override;
};

#endif