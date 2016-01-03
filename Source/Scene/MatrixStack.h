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
#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <vector>
#include <glm/glm.hpp>


class MatrixStack
{
protected:
    std::vector<glm::mat4> m_Matrices;
public:
    MatrixStack();

    void Clear();

    void Push(glm::mat4 matrix);
    glm::mat4 Pop();

    const glm::mat4& GetTopMatrix() const;
    unsigned int Count() const;
};

#endif