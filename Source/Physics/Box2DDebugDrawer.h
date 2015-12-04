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
                glVertex2f(vertices[i].x * 40.0f, vertices[i].y * 40.0f);
            }
        glEnd();
    }

    void DrawSolidPolygon(const b2Vec2* vertices, int vertexCount, const b2Color& color)
    {
        //glBegin(GL_LINE_STRIP);
        glBegin(GL_TRIANGLE_FAN);
        for (int i = 0; i < vertexCount; ++i)
        {
            glColor3f(color.r, color.g, 0.0);
            glVertex2f(vertices[i].x * 40.0f, vertices[i].y * 40.0f);
        }
        glEnd();
    }

    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) 
    {
        glColor3f(color.r, color.g, 0.0);
        glBegin(GL_POLYGON);           
		    for(double i = 0; i < 2 * 3.14; i += 3.14 / 12) //<-- Change this Value
 			    glVertex2f(center.x + cos(i) * radius, center.y + sin(i) * radius);
		glEnd();
    }

    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) 
    {
        glColor3f(color.r, color.g, 0.0);
        glBegin(GL_POLYGON);           
		    for(double i = 0; i < 2 * 3.14; i += 3.14 / 12) //<-- Change this Value
 			    glVertex2f(center.x * 40.0f + cos(i) * radius * 40.0f, center.y * 40.0f + sin(i) * radius * 40.0f);
		glEnd();
    }

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) 
    {
        glLineWidth(5); 
        glBegin(GL_LINES);
            glColor3f(color.r, color.g, 0.0);
            glVertex2f(p1.x * 40.0f, p1.y * 40.0f);
            glVertex2f(p2.x * 40.0f, p2.y * 40.0f);
        glEnd();
    }

    void DrawTransform(const b2Transform& xf) {}
};

#endif