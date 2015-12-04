#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H
#include "SceneNode.h"
#include "../Renderer/Animation.h"
#include <memory>
#include <map>

class Scene;
class SpriteNode : public SceneNode
{
protected:
	bool m_Animation;
	std::string m_ActiveAnimation;
	std::map<std::string, std::shared_ptr<Animation>> m_Animations; // <anim-state, animation>
public:
    SpriteNode(unsigned int ActorID, std::string name, std::string renderPass, glm::vec2 position, int depth, glm::vec2 scale = glm::vec2(1.0), float rotation = 0.0f);

    virtual void Initialize(Scene *scene);
    virtual void Update(Scene *scene, float deltaTime);
    virtual void Render(Scene *scene);

	void					   SetAnimation(bool enable);
	bool					   HasAnimation();
	void					   AddAnimation(std::shared_ptr<Animation> animation, std::string state = "default");
	std::shared_ptr<Animation> GetAnimation(std::string state);
	void					   ActivateAnimation(std::string state);	


    void ActorMoved(std::shared_ptr<IEventData> eventData);
};

#endif