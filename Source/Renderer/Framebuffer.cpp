#include "Framebuffer.h"

#include <iostream>

Framebuffer::Framebuffer(GLuint width, GLuint height) : m_Width(width), m_Height(height)
{ 
    m_ColorBuffer.reset(new Texture2D);
    m_DepthBuffer.reset(new Texture2D);
    // Generate/configure framebuffer
    glGenFramebuffers(1, &m_FBO);
    // - Configure color buffer
    m_ColorBuffer->InternalFormat = GL_RGB16F; // Use floating point precision color attachment
    m_ColorBuffer->FilterMin = GL_LINEAR;
    m_ColorBuffer->Generate(width, height, NULL);
    // - Configure depth buffer
    m_DepthBuffer->ImageFormat = GL_DEPTH_COMPONENT;
    m_DepthBuffer->InternalFormat = GL_DEPTH_COMPONENT;
    m_DepthBuffer->FilterMin = GL_DEPTH_COMPONENT;
    m_DepthBuffer->Generate(width, height, NULL);
    // Attach to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer->ID, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer->ID, 0);
    // Check if framebuffer is succesfully generated
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete; Check framebuffers!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Texture2D> const Framebuffer::GetColorBuffer()
{
    return m_ColorBuffer;
}

std::shared_ptr<Texture2D> const Framebuffer::GetDepthBuffer()
{
    return m_DepthBuffer;
}

// Update the texture used as the color buffer
void Framebuffer::UpdateColorBufferTexture(std::shared_ptr<Texture2D> texture)
{
    m_ColorBuffer = texture;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer->ID, 0);
}

// Binds the current framebuffer (without clearing)
void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}
    
// Bind to framebuffer and clear buffers 
void Framebuffer::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Unbind framebuffer to store data in buffers
void Framebuffer::EndRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}