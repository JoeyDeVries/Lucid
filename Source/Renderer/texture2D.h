#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/glew.h>


class Texture2D
{
// DATA
public:
    // Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    GLuint ID             = 0;
    // Texture image dimensions
    GLuint Width          = 0; // Width of loaded image in pixels
    GLuint Height         = 0; // Height of loaded image in pixels
    // Texture Format
    GLenum InternalFormat = GL_RGB; // Format of texture object
    GLenum ImageFormat    = GL_RGB; // Format of loaded image
    // Texture configuration
    GLenum FilterMin      = GL_LINEAR; // Filtering mode if texture pixels < screen pixels
    GLenum FilterMax      = GL_LINEAR; // Filtering mode if texture pixels > screen pixels
    GLenum WrapS          = GL_REPEAT; // Wrapping mode on S axis
    GLenum WrapT          = GL_REPEAT; // Wrapping mode on T axis
private:

// FUNCTIONS
public:
         Texture2D(void);
    // Generates texture from array of image data
    void Generate(GLuint width, GLuint height, const unsigned char *data, GLboolean mipmaps = GL_FALSE);
    // Binds the texture as the current active GL_TEXTURE_2D texture object and texture unit if given
    void Bind(GLbyte unit = -1) const;
private:
};

#endif