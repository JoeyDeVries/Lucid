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
#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

Shader &Shader::Use()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, std::string directory)
{
	// first pre-process shaders with preprocessor (includes, pragma defines etc.)
	std::string vsCodeProcessed = PreProcess(std::string(vertexSource), directory);
	std::string fsCodeProcessed = PreProcess(std::string(fragmentSource), directory);
	const GLchar* vsCode = vsCodeProcessed.c_str();
	const GLchar* fsCode = fsCodeProcessed.c_str();
	
	// then process pre-processed shader source
    GLuint sVertex, sFragment;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vsCode, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fsCode, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // Shader Program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, sVertex);
    glAttachShader(this->ID, sFragment);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

std::string Shader::PreProcess(std::string shaderCode, std::string directory)
{
	std::stringstream input(shaderCode);
	std::stringstream output;
	std::string line;
	while(std::getline(input, line))
	{
		if(line.substr(0, 15) == "#pragma include")
		{
			std::string filepath = line.substr(16);
			if(filepath != "")
				filepath = directory + "/" + filepath;
			std::ifstream file(filepath);			
			while(std::getline(file, line))
				output << line << std::endl;
		}
		else
		{
			output << line << std::endl;
		}
	}
	return output.str();
}

void Shader::SetFloat(std::string name, GLfloat value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetInteger(std::string name, GLint value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetVector2f(std::string name, GLfloat x, GLfloat y, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}
void Shader::SetVector2f(std::string name, const glm::vec2 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}
void Shader::SetVector3f(std::string name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}
void Shader::SetVector3f(std::string name, const glm::vec3 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::SetVector4f(std::string name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}
void Shader::SetVector4f(std::string name, const glm::vec4 &value, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix3(std::string name, const glm::mat3 &matrix, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix3fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::SetMatrix4(std::string name, const glm::mat4 &matrix, GLboolean useShader)
{
    if (useShader)
        this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}