#ifndef TEXT_ON_TOUCH_COMPONENT_H
#define TEXT_ON_TOUCH_COMPONENT_H
#include "ActorComponent.h"
#include "../Communication/IEventData.h"
#include <string>

class TextOnTouchComponent : public ActorComponent
{
private:
    std::string m_DisplayText;
    bool m_IsActive;
public:
    TextOnTouchComponent();
    ~TextOnTouchComponent();

    std::string GetDisplayText();
    bool GetIsActive();
    void SetDisplayText(std::string text);

    bool VInit();
    void VUpdate(float deltaTime);

    void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
    void OnPostCollisionRemove(std::shared_ptr<IEventData> eventData);
};

#endif