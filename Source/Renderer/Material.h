#ifndef MATERIAL_H
#define MATERIAL_H

#include "shader.h"
#include "texture2D.h"
#include <glm/glm.hpp>
#include <memory>

class Material
{
private:
    std::shared_ptr<Shader>    m_Shader;
    std::shared_ptr<Texture2D> m_Diffuse;
    std::shared_ptr<Texture2D> m_Specular;
    std::shared_ptr<Texture2D> m_Normal;
	glm::vec3				   m_ColorOverride;
	float					   m_Alpha;
public:
    Material();

    void SetShader(std::shared_ptr<Shader> shader);
    void SetDiffuse(std::shared_ptr<Texture2D> diffuse);
    void SetSpecular(std::shared_ptr<Texture2D> specular);
    void SetNormal(std::shared_ptr<Texture2D> normal);
	void SetColorOverride(const glm::vec3& color);
	void SetAlpha(float alpha);

    std::shared_ptr<Shader>    GetShader()		  { return m_Shader; }
    std::shared_ptr<Texture2D> GetDiffuse()		  { return m_Diffuse; }
    std::shared_ptr<Texture2D> GetSpecular()	  { return m_Specular; }
    std::shared_ptr<Texture2D> GetNormal()		  { return m_Normal; }
	const glm::vec3&		   GetColorOverride() { return m_ColorOverride; }
	const float				   GetAlpha()		  { return m_Alpha; }

    bool HasDiffuse()  { return (m_Diffuse  ? true : false); }
    bool HasSpecular() { return (m_Specular ? true : false); }
    bool HasNormal()   { return (m_Normal   ? true : false); }

    void Initialize(); // sets texture units corresponding to active textures
    void PreRender();
};



#endif