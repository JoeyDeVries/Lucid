#ifndef GUI_GAME_MENU_H
#define GUI_GAME_MENU_H
#include "GUIContainer.h"

class GUIGameMenu : public GUIContainer
{
private:
    std::shared_ptr<Texture2D> m_BackgroundTexture;
public:
    GUIGameMenu();

    bool Init() override;
    void Update(float deltaTime) override;
    void RenderBackground(Renderer *renderer, TextRenderer *textRenderer) override; // override background rendering

    void OnActivate() override;
    void OnDeactivate() override;

    void ButtonPressed(std::shared_ptr<GUIButton> pButton) override;
};


#endif