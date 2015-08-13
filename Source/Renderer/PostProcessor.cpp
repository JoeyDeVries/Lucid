#include "PostProcesser.h"
#include "Renderer.h"

#include "../Resources/ResourceManager.h"
#include "../Application/GameApplication.h"

PostProcessor::PostProcessor()
{
  
}

void PostProcessor::Initialize()
{
    m_Shader = ResourceManager::GetInstance()->LoadShader("postprocessor", "shaders/post_processor.vs", "shaders/post_processor.frag");
    m_Framebuffer.reset(new Framebuffer(GameApplication::GetInstance()->ScreenWidth(), GameApplication::GetInstance()->ScreenHeight()));
}

std::shared_ptr<Shader> const PostProcessor::GetShader()
{
    return m_Shader;
}

void PostProcessor::PreRender()
{
    m_Framebuffer->BeginRender();
}

void PostProcessor::PostRender()
{
    m_Framebuffer->EndRender();
}

void PostProcessor::RenderScreenQuad(Renderer* renderer)
{
    m_Shader->Use();
    m_Framebuffer->GetColorBuffer()->Bind(0);
    renderer->RenderQuad();
}

void PostProcessor::EnableEffect(POST_PROCESS_EFFECT effect, float duration)
{
    m_Shader->SetInteger("effect", effect, true);
}