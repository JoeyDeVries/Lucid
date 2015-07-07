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

const GLuint GAME_WIDTH = 800, GAME_HEIGHT = 600;

int main(int argc, char *argv[])
{
    // - initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(GAME_WIDTH, GAME_HEIGHT, "Lantarn - prototype v0.2153", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "-- failed to initialize GLFW..." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // - initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "-- failed to initialize GLEW..." << std::endl;
        glfwTerminate();
        return -1;
    }

    // OpenGL settings
    glViewport(0, 0, GAME_WIDTH, GAME_HEIGHT);
    glClearColor(0.3f, 0.2f, 0.1f, 1.0f);

    // - game loop
    while (!glfwWindowShouldClose(window))
    {
        // - events
        glfwPollEvents();
        
        // - frame init
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // - frame end: double buffer swap
        glfwSwapBuffers(window);
    }




    glfwTerminate();
    return 0;
}