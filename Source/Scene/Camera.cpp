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
#include "Camera.h"

Camera::Camera()
{
    m_Projection = glm::mat4();
    m_View = glm::mat4();
}


void Camera::SetProjection(float width, float height, float near, float far)
{
    m_Width = width;
    m_Height = height;
    m_Projection = glm::ortho(0.0f, width, height, 0.0f, near, far);
}

void Camera::SetTarget(std::shared_ptr<SceneNode> target)
{
	m_Target = target;
	m_CameraCenter = target->GetPosition();
    m_CurrentPos = m_CameraCenter;
}


// Camera system is bounding box based. This states that an AABB is centered
// around the target node and the camera moves accordingly when the target 
// reaches the edges of the AABB. More dynamic/flexible than a center target
// camera system. System can easily be tweaked from here.
void Camera::CalculateViewMatrix()
{
	glm::vec2 center = glm::vec2(m_Width * 0.5f, m_Height * 0.5f);
    if (m_Target)
    {
        glm::vec2 targetPos = m_Target->GetPosition();
        glm::vec2 targetScale = m_Target->GetScale();
        // define a bounding box around the screen's center
        glm::vec2 AABBCenter = m_CameraCenter;
        glm::vec2 AABBHalfWidths = glm::vec2(m_Width * 0.2f, m_Height * 0.2f);
        // - check if target is outside AABB and if so change center accordingly 
        if (targetPos.x + targetScale.x * 0.5 >= AABBCenter.x + AABBHalfWidths.x) // target at right side of box
            m_CameraCenter.x += (targetPos.x + targetScale.x * 0.5) - (AABBCenter.x + AABBHalfWidths.x);
        else if (targetPos.x + targetScale.x * 0.5 <= AABBCenter.x - AABBHalfWidths.x) // target at left side of box
            m_CameraCenter.x -= (AABBCenter.x - AABBHalfWidths.x) - (targetPos.x + targetScale.x * 0.5);
        if (targetPos.y + targetScale.y * 0.5 >= AABBCenter.y + AABBHalfWidths.y) // target at bottom side of box
            m_CameraCenter.y += (targetPos.y + targetScale.y * 0.5) - (AABBCenter.y + AABBHalfWidths.y);
        else if (targetPos.y + targetScale.y * 0.5 <= AABBCenter.y - AABBHalfWidths.y) // target at top side of box
            m_CameraCenter.y -= (AABBCenter.y - AABBHalfWidths.y) - (targetPos.y + targetScale.y * 0.5);

        //slowly align the camera back to the target center (either always on, or after a given amount of time in the bounding box)
        m_CurrentPos = glm::mix(m_CurrentPos, m_CameraCenter, 0.04f);
    }
    glm::mat4 view;
	//view = glm::translate(view, -glm::vec3(-center.x + targetPos.x + 0.5 * targetScale.x, -center.y + targetPos.y + 0.5 * targetScale.y, 0.0)); // center at target matrix
	view = glm::translate(view, -glm::vec3(-center.x + m_CurrentPos.x, -center.y + m_CurrentPos.y, 0.0));
    m_View = view;
}

glm::mat4 Camera::GetProjection()
{
	return m_Projection;
}

glm::mat4 Camera::GetView()
{
	return m_View;
}

BoundingBox Camera::GetBoundingBox()
{
	BoundingBox box;
	box.Center = m_CameraCenter;
	box.HalfExtents = glm::vec2(m_Width * 0.5f, m_Height * 0.5f);
	return box;
}

