#ifndef RENDERER_H
#define RENDERER_H
#include "../Scene/SceneNode.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Renderer
{
private:
    GLuint m_VAO, m_VBO;
    void configureQuad();
public:
    Renderer();
    virtual void Initialize();
    virtual void PreRender();
    virtual void RenderQuad();
};

#endif