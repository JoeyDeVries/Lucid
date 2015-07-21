#include "Renderer.h"
 
Renderer::Renderer()
{

}

void Renderer::Initialize()
{
    // Configure default OpenGL state
    glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
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