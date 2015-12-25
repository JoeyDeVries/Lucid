#include "Renderer.h"
 
Renderer::Renderer()
{
    m_PostProcessor = new PostProcessor;
}

Renderer::~Renderer()
{
    delete m_PostProcessor;
}

void Renderer::Initialize()
{
    // Initialize post-processor
    m_PostProcessor->Initialize();

    // Configure default OpenGL state
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Configure default shape forms
    configureQuad();
}

void Renderer::PreRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderQuad()
{
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::configureQuad()
{
    GLfloat vertices[] = {
        // Position // Texture Coords
        0.0f, 1.0f, 0.0f, 1.0f, // Top Left
        0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
        1.0f, 1.0f, 1.0f, 1.0f, // Top Right
        1.0f, 0.0f, 1.0f, 0.0f  // Bottom Right 
    };
    glGenVertexArrays(1, &m_VAO); glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // Unbind VAO
}

void Renderer::PrePostProcessRender()
{
    m_PostProcessor->PreRender();
}

void Renderer::PostPostProcessRender()
{
    m_PostProcessor->PostRender();
}

void Renderer::PostProcessQuadRender()
{
    m_PostProcessor->RenderScreenQuad(this);
}

void Renderer::EnableEffect(POST_PROCESS_EFFECT effect)
{
    m_PostProcessor->EnableEffect(effect);
}
void Renderer::DisableEffect(POST_PROCESS_EFFECT effect)
{
    m_PostProcessor->DisableEffect(effect);
}