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
#ifndef CAMERA_H
#define CAMERA_H
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SceneNode.h"

struct BoundingBox {
	glm::vec2 Center;
	glm::vec2 HalfExtents;
};

class Camera
{
protected:
    float m_Width, m_Height;
    glm::mat4 m_Projection;
    glm::mat4 m_View;
    std::shared_ptr<SceneNode> m_Target;
	glm::vec2 m_CameraCenter;
    glm::vec2 m_CurrentPos; // store current pos for adaptive camera movement to m_CameraCenter
public:
    Camera();

    void SetTarget(std::shared_ptr<SceneNode> target);

    void SetProjection(float width, float height, float near = 0.0f, float far = 10.0f);
    void CalculateViewMatrix();
	
	BoundingBox GetBoundingBox();

    glm::mat4 GetProjection();
    glm::mat4 GetView();
};




#endif