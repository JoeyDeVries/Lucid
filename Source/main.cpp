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
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// SOIL
#include <SOIL.h>

// Lantarn
#include "Application/GameApplication.h"
#include "Resources/ResourceManager.h"
#include "Renderer/shader.h"
#include "Renderer/texture2D.h"
#include "Components/Actor.h"

// Physics test
#include <Box2D/Box2D.h>
b2Vec2 test;
const GLuint GAME_WIDTH = 800, GAME_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_move_callback(GLFWwindow *window, double x, double y);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

int main(int argc, char *argv[])
{
    // - initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "Lantern - prototype v0.2153", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "-- failed to initialize GLFW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // - callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // - initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "-- failed to initialize GLEW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glGetError();

    // OpenGL settings
    glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
    glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize
    GameApplication::GetInstance()->Initialize(GAME_WIDTH, GAME_HEIGHT); // TODO: replace with current framebuffer size, not initial size

    // deltatime variables
    GLfloat currentFrame = glfwGetTime(), deltaTime = 0.0, lastFrame = 0.0;

	GLuint fps = 0;
	GLfloat time = 0.0f;
    // - game loop
    while (!glfwWindowShouldClose(window) && GameApplication::GetInstance()->GetActive())
    {
        // - calculate time spent on last frame
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		
		time += deltaTime;
		++fps;
		if (time >= 1.0f)
		{
			time = 1.0 - time;
			glfwSetWindowTitle(window, std::string("Lantern - prototype v0.2153 || FPS: " + std::to_string(fps)).c_str());
			fps = 0;
		}
		
		

        // - events
        glfwPollEvents();

        // - update game 
        GameApplication::GetInstance()->Update(deltaTime);

        //// - render game
        GameApplication::GetInstance()->Render();
        
        // - frame end: double buffer swap
        glfwSwapBuffers(window);
    }

    GameApplication::GetInstance()->CleanUp();   



    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            GameApplication::GetInstance()->ProcessKeyboardDown(key);
        else if(action == GLFW_RELEASE)
            GameApplication::GetInstance()->ProcessKeyboardUp(key);
    }
}

void mouse_move_callback(GLFWwindow *window, double x, double y)
{
    GameApplication::GetInstance()->ProcessMouseMove(x, y);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if(action == GLFW_PRESS)
        GameApplication::GetInstance()->ProcessMouseClick(button = GLFW_MOUSE_BUTTON_LEFT);
}
