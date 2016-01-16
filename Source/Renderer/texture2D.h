/*******************************************************************
** Copyright (C) 2014-2015 {Joey de Vries} {joey.d.vries@gmail.com}
**
** This code is part of Lucid.
** https://github.com/JoeyDeVries/Lucid
**
** Lucid is free software: you can redistribute it and/or modify it
** under the terms of the CC BY-NC 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
*******************************************************************/
#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#define GLEW_STATIC
#include <GL/glew.h>

/*
    Render state required for storing and rendering a 2D texture image.
*/
class Texture2D
{
public:
    // Texture render state
    GLuint ID             = 0; // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    // Texture image dimensions
    GLuint Width          = 0; // width of loaded image in pixels
    GLuint Height         = 0; // height of loaded image in pixels
    // Texture Format
    GLenum InternalFormat = GL_RGB; // format of texture object
    GLenum ImageFormat    = GL_RGB; // format of loaded image
    // Texture configuration
    GLenum FilterMin      = GL_LINEAR;          // filtering mode if texture pixels < screen pixels
    GLenum FilterMax      = GL_LINEAR;          // filtering mode if texture pixels > screen pixels
    GLenum WrapS          = GL_MIRRORED_REPEAT; // wrapping mode on S axis
    GLenum WrapT          = GL_MIRRORED_REPEAT; // wrapping mode on T axis
    
    Texture2D();

    // generates texture from array of image data
    void Generate(GLuint width, GLuint height, const unsigned char *data, GLboolean mipmaps = GL_FALSE);
    // binds the texture as the current active GL_TEXTURE_2D texture object and texture unit if given
    void Bind(GLbyte unit = -1) const;
};
#endif