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
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

#include <memory>

class Texture2D;
class Shader;

/*
    Contains all relevant render state of rendering a game entity. This includes
    textures used, its alpha value, potential color overrides and the shader used
    for rendering its graphics.
*/
class Material
{
private:
    std::shared_ptr<Shader>    m_Shader;        // shader used for rendering this material
    std::shared_ptr<Texture2D> m_Diffuse;       // the diffuse material color
    std::shared_ptr<Texture2D> m_Specular;      // the specular material color
    std::shared_ptr<Texture2D> m_Normal;        // the normal material color
	glm::vec3				   m_ColorOverride; // a color to override the final render color (direct multiplication)
	float					   m_Alpha;         // the alpha value of the material
public:
    Material();

    // getters
    std::shared_ptr<Shader>    GetShader()        { return m_Shader; }
    std::shared_ptr<Texture2D> GetDiffuse()       { return m_Diffuse; }
    std::shared_ptr<Texture2D> GetSpecular()      { return m_Specular; }
    std::shared_ptr<Texture2D> GetNormal()        { return m_Normal; }
    const glm::vec3&		   GetColorOverride() { return m_ColorOverride; }
    const float				   GetAlpha()         { return m_Alpha; }
    // setters
    void SetShader(std::shared_ptr<Shader> shader);
    void SetDiffuse(std::shared_ptr<Texture2D> diffuse);
    void SetSpecular(std::shared_ptr<Texture2D> specular);
    void SetNormal(std::shared_ptr<Texture2D> normal);
	void SetColorOverride(const glm::vec3& color);
	void SetAlpha(float alpha);
   
    // whether the material has a diffuse texture
    bool HasDiffuse()  { return (m_Diffuse  ? true : false); }
    // whether the material has a specular texture
    bool HasSpecular() { return (m_Specular ? true : false); }
    // whether the material has a normal texture
    bool HasNormal()   { return (m_Normal   ? true : false); }

    // initializes the material by setting texture units corresponding to textures in use
    void Initialize(); 
    // sets proper shader settings and binds relevant material textures
    void PreRender();
};
#endif