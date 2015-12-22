#include "GUIGameMenu.h"
#include "../Application/GameApplication.h"

GUIGameMenu::GUIGameMenu()
{

}

bool GUIGameMenu::Init()
{
    return true;
}

void GUIGameMenu::Update(float deltaTime)
{

}

void GUIGameMenu::RenderBackground(Renderer *renderer, TextRenderer *textRenderer)
{

}

void GUIGameMenu::OnActivate()
{
    //GameApplication::GetInstance()->GetRenderer()->EnableBlur();
}

void GUIGameMenu::OnDeactivate()
{
    //GameApplication::GetInstance()->GetRenderer()->DisableBlur();
}
void GUIGameMenu::ButtonPressed(std::shared_ptr<GUIButton> pButton)
{

}