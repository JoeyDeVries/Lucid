#include "Animation.h"


Animation::Animation(std::vector<AnimationData> &data)
{
	//m_SpriteSheet_Diffuse = spriteSheet_Diffuse;
	//m_SpriteSheet_Specular = spriteSheet_Specular;
	//m_SpriteSheet_Normal = spriteSheet_Normal;
	m_AnimationData = data;
}

void Animation::SetDuration(float duration)
{
	m_Duration = duration;
}

void Animation::Init(std::shared_ptr<Shader> shader)
{
	//shader->SetInteger("NrFrames", m_AnimationData.size(), true);
	//shader.SetInteger("SpriteSheet_Diffuse", 0);
	//shader.SetInteger("SpriteSheet_Specular", 1);
	//shader.SetInteger("SpriteSheet_Normal", 2);
}

void Animation::Normalize()
{
	for (auto it = m_AnimationData.begin(); it != m_AnimationData.end(); ++it)
	{
		float totalWidth  = it->Width * m_AnimationData.size();
		float totalHeight = 128.0f; // TODO (Give texture width/height as input)
		it->XOffset = it->XOffset / totalWidth;
		it->YOffset = 1.0 - it->YOffset / totalHeight;
		it->Width   = it->Width / totalWidth;
		it->Height  = it->Height / totalHeight;
	}
}

void Animation::Update(float deltaTime)
{
	m_CurrentTime += deltaTime;
	m_CurrentTime = fmod(m_CurrentTime, m_Duration);
}

void Animation::ToShader(std::shared_ptr<Shader> shader)
{
	// calculate current frame
	float unit = m_CurrentTime / m_Duration;
	int nr_frames = m_AnimationData.size();
	int current_frame = static_cast<int>(unit * nr_frames);
	// send relevant animation data to shader
	shader->SetVector4f("animationData", m_AnimationData[current_frame].ToVec4(), true);
	// bind relevant sheets
	/*m_SpriteSheet_Diffuse.Bind(0); // will be done by Material
	m_SpriteSheet_Diffuse.Bind(1);
	m_SpriteSheet_Diffuse.Bind(2);*/
}
