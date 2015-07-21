#include "Material.h"


Material::Material()
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
        m_Diffuse->Bind(0);
    if (HasSpecular())
        m_Specular->Bind(1);
    if (HasNormal())
        m_Normal->Bind(2);
}