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

#include <glm/glm.hpp>

#include <vector>

/*
    Hosts a stack of transformation matrices where each is multiplied by the 
    transformation matrix below. The top-most matrix of the stack thus holds
    all the previously pushed transformations. This works great in combination
    with a scene graph such that parent nodes' transformations are included
    in their children's world transformation matrices.
*/
class MatrixStack
{
protected:
    std::vector<glm::mat4> m_Matrices; // the stack of matrices 
public:
    MatrixStack();

    // clears the stack of matrices to a vector holding only the identity matrix
    void Clear();

    // pushes a matrix on top of the stack, multiplying it by the previous top of the stack
    void Push(glm::mat4 matrix);
    // pops the top matrix of the stack
    glm::mat4 Pop();

    // returns the transformation matrix currently at the top of the stack
    const glm::mat4& GetTopMatrix() const;

    // returns the amount of matrices currently in the matrix stack
    unsigned int Count() const;
};
#endif