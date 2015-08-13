#ifndef POST_PROCESSER_H
#define POST_PROCESSER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "shader.h"
#include "Framebuffer.h"
#include <memory>

class Renderer;

enum POST_PROCESS_EFFECT
{
    POST_PROCESS_NONE,
    POST_PROCESS_WHITESCALE,
    POST_PROCESS_BLUR,
    POST_PROCESS_SHAKE,
    POST_PROCESS_FLASH,
};

class PostProcessor
{
    std::shared_ptr<Shader> m_Shader;
    std::unique_ptr<Framebuffer> m_Framebuffer;
public:
    PostProcessor();

    std::shared_ptr<Shader> const GetShader();
    
    void Initialize();
    void PreRender();
    void PostRender();
    void RenderScreenQuad(Renderer* renderer);

    void EnableEffect(POST_PROCESS_EFFECT effect, float duration = 0.0f);
};


#endif