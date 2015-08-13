#ifndef RENDERER_H
#define RENDERER_H
#include "../Scene/SceneNode.h"
#include "PostProcesser.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
private:
    PostProcessor* m_PostProcessor;

    GLuint m_VAO, m_VBO;
    void configureQuad();
public:
    Renderer();
    ~Renderer();
    virtual void Initialize();
    virtual void PreRender();
    virtual void RenderQuad();

    virtual void PrePostProcessRender();
    virtual void PostPostProcessRender();
    virtual void PostProcessQuadRender();
};

#endif