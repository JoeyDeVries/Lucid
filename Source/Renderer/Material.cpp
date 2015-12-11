#include "Material.h"


Material::Material() : m_ColorOverride(1.0, 1.0, 1.0), m_Alpha(1.0)
{

}

void Material::SetShader(std::shared_ptr<Shader> shader)
{
    m_Shader = shader;
}

void Material::SetDiffuse(std::shared_ptr<Texture2D> diffuse)
{
    m_Diffuse = diffuse;
}

void Material::SetSpecular(std::shared_ptr<Texture2D> specular)
{
    m_Specular = specular;
}

void Material::SetNormal(std::shared_ptr<Texture2D> normal)
{
    m_Normal = normal;
}

void Material::SetColorOverride(const glm::vec3& color)
{
	m_ColorOverride = color;
}

void Material::SetAlpha(float alpha)
{
	m_Alpha = alpha;
}

void Material::Initialize()
{
    // Set texture units
    m_Shader->SetInteger("tDiffuse", 0, true);
    if (HasSpecular())
        m_Shader->SetInteger("tSpecular", 1);
    if (HasNormal())
        m_Shader->SetInteger("tNormal", 2);
}

void Material::PreRender()
{
	if (HasDiffuse())
	{
		m_Shader->SetVector4f("ColorOverride", glm::vec4(m_ColorOverride, m_Alpha), true);
		m_Diffuse->Bind(0);
	}
    if (HasSpecular())
        m_Specular->Bind(1);
    if (HasNormal())
        m_Normal->Bind(2);
}