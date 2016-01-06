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

#include <glm/glm.hpp>

#include <memory>

class SceneNode;

/*
    Defines a 2D AABB for fast collision/boundary checks
*/
struct BoundingBox {
	glm::vec2 Center;
	glm::vec2 HalfExtents;
};

/* 
    Follows a target node by re-calculating a proper view matrix each frame. Uses adaptive
    camera movement based around a Bounding Box region surrounding the target node.
*/
class Camera
{
protected:
    std::shared_ptr<SceneNode> m_Target; // the target node to follow

    float     m_Width, m_Height; // the width and height of the game
    glm::mat4 m_Projection;      // the projection matrix of the scene
    glm::mat4 m_View;            // the per-frame generated view matrix
	glm::vec2 m_CameraCenter;    // the center of the camera within the bounding box
    glm::vec2 m_CurrentPos;      // stores the current position for adaptive camera movement to m_CameraCenter
public:
    Camera();

    // getters
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    // calculates the bounding box aligned to the target scene node
    BoundingBox GetBoundingBox();

    // specifies the target scene node to follow
    void SetTarget(std::shared_ptr<SceneNode> target);
    // sets the projection matrix of the scene
    void SetProjection(float width, float height, float near = 0.0f, float far = 10.0f);
    // (re-)calculates the view matrix
    void CalculateViewMatrix();	
};
#endif