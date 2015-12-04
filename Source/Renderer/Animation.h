#ifndef ANIMATION_H
#define ANIMATION_H
#include "texture2D.h"
#include "shader.h"
#include <vector>
#include <memory>

struct AnimationData
{
	float XOffset, YOffset;
	float Width, Height;

	glm::vec4 ToVec4() 
	{
		return glm::vec4(XOffset, YOffset, Width, Height);
	}
};

class Animation
{
private:
	std::vector<AnimationData> m_AnimationData;
	float m_Duration;
	float m_CurrentTime; // transform to unit interval in calculations or irrelevant? -> (scaled to m_Duration for now)
public:
	Animation(std::vector<AnimationData> &data);

	void SetDuration(float duration);

	void Init(std::shared_ptr<Shader> shader);
	void Normalize(); // normalizes animation value from pixels to render-texture coordinates
	void Update(float deltaTime);
	void ToShader(std::shared_ptr<Shader> shader);

};

#endif