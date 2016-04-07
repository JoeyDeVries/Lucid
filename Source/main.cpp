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
// 3rd party
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// lucid
#include "Application/GameApplication.h"

// standard
#include <iostream>

const GLuint GAME_WIDTH = 1280, GAME_HEIGHT = 720;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_move_callback(GLFWwindow *window, double x, double y);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

/*
    The main entry point for Lucid. Here the cross-platform window is configured and an
    OpenGL context is requested. After a window is configured, the game is initialized
    and the game loop is run. The windowing system also continously delegates keyboard
    and mouse events to the game.
*/
int main(int argc, char *argv[])
{
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "Lucid - Game/Engine - Joey de Vries", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "-- failed to initialize GLFW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // configure event callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "-- failed to initialize GLEW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    // call glGetError once to remove the GLEW error code that's always returned due to a bug in GLEW; now all 
    // glGetError codes belong to Lucid only.
    glGetError();

    // initialize the game and all its game sub-systems
    GameApplication::GetInstance()->Initialize(GAME_WIDTH, GAME_HEIGHT);

    // time management
    GLfloat currentFrame = glfwGetTime(), deltaTime = 0.0, lastFrame = 0.0;
    GLfloat time = 0.0f;
    GLuint  fps = 0;

    // - game loop
    while (!glfwWindowShouldClose(window) && GameApplication::GetInstance()->GetActive())
    {
        // - calculate time spent on last frame
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // - periodcally display the FPS the game is running in
        time += deltaTime;
        ++fps;
        if (time >= 1.0f)
        {
            time = 1.0 - time;
            glfwSetWindowTitle(window, std::string("Lucid - Game/Engine - Joey de Vries || FPS: " + std::to_string(fps)).c_str());
            fps = 0;
        }

        // - poll events
        glfwPollEvents();

        // - update game 
        GameApplication::GetInstance()->Update(deltaTime);

        // - render game
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