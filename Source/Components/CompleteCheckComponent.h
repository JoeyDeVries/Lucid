#ifndef LEVEL_COMPLETE_CHECK_H
#define LEVEL_COMPLETE_CHECK_H
#include "ActorComponent.h"
#include "../Communication/IEventData.h"
#include "../Physics/Box2DPhysics.h"
#include "Event_LevelComplete.h"

class CompleteCheckComponent : public ActorComponent
{
private:
	std::string m_NextLevelString;
public:
	CompleteCheckComponent();
	~CompleteCheckComponent();

    std::string GetNextLevelString();
    void SetNextLevelString(std::string levelPath);

	virtual bool VInit();
	virtual void VUpdate(float delta);


	void OnPostCollisionAdd(std::shared_ptr<IEventData> eventData);
};


#endif