/* ***************************************************
/* Developed by: Joey de Vries
/* Last-update:  05-07-2015
/* License:      CC-3.0 A-NC
*************************************************** */
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

int main(int argc, char *argv[])
{
    // - initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "Lantarn - prototype v0.2153", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "-- failed to initialize GLFW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // - callbacks
    glfwSetKeyCallback(window, key_callback);


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

    // - game loop
    while (!glfwWindowShouldClose(window))
    {
        // - calculate time spent on last frame
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // - events
        glfwPollEvents();

        // - update game 
        GameApplication::GetInstance()->Update(deltaTime);

        //// - render game
        GameApplication::GetInstance()->Render();
        
        // - frame end: double buffer swap
        glfwSwapBuffers(window);
    }

    



    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, 
    // closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            GameApplication::GetInstance()->ProcessKeyboardDown(key);
        else if(action == GLFW_RELEASE)
            GameApplication::GetInstance()->ProcessKeyboardUp(key);
    }
}