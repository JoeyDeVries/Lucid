#ifndef BOX_2D_DEBUG_DRAWER_H
#define BOX_2D_DEBUG_DRAWER_H
#include <Box2D/Box2D.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/GL.h>

class Box2DDebugDrawer : public b2Draw
{
public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) 
    {
        glBegin(GL_TRIANGLE_FAN);
            for (int i = 0; i < vertexCount; ++i)
            {
                glColor3f(color.r, color.g, 0.0);
                glVertex2f(vertices[i].x * 25.0f, vertices[i].y * 25.0f);
            }
        glEnd();
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
    {
        glBegin(GL_LINE_STRIP);
        //glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < vertexCount; ++i)
        {
            glColor3f(color.r, color.g, 0.0);
            glVertex2f(vertices[i].x * 25.0f, vertices[i].y * 25.0f);
        }
        glEnd();
    }

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
    {
        glLineWidth(5); 
        glBegin(GL_LINES);
            glColor3f(color.r, color.g, 0.0);
            glVertex2f(p1.x * 25.0f, p1.y * 25.0f);
            glVertex2f(p2.x * 25.0f, p2.y * 25.0f);
        glEnd();
    }

    void DrawTransform(const b2Transform& xf) {}
};

#endif