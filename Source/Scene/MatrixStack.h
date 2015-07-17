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
    void Push(glm::mat4 matrix);
    glm::mat4 Pop();
    const glm::mat4& GetTopMatrix() const;
    unsigned int Count() const;
};

#endif