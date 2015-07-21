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

const GLuint GAME_WIDTH = 800, GAME_HEIGHT = 600;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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


    // PROTOTYPE CODE
    // --------------
    // Shaders
    const GLchar* vertexShader = "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 model;\n"
        "out vec2 TexCoords;\n"
        "void main()\n"
        "{\n"
        "gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);\n"
        "TexCoords = vertex.zw;\n"
        "}\n";
    const GLchar* fragmentShader = "#version 330 core\n"
        "out vec4 color;\n"
        "in vec2 TexCoords;\n"
        "uniform sampler2D sprite;\n"
        "void main()\n"
        "{\n"
        "color = vec4(texture(sprite, TexCoords).rgba);\n"
        "}\n";
    Shader shaderSprite; 
    shaderSprite.Compile(vertexShader, fragmentShader);
    // Textures
    Texture2D texBlock, texBackground, texPlayer;
    int width, height;
    unsigned char* image = SOIL_load_image("textures/block.png", &width, &height, 0, SOIL_LOAD_RGB);
    texBlock.Generate(width, height, image);
    SOIL_free_image_data(image);
    image = SOIL_load_image("textures/background.png", &width, &height, 0, SOIL_LOAD_RGB);
    texBackground.Generate(width, height, image);
    SOIL_free_image_data(image);
    image = SOIL_load_image("textures/player.png", &width, &height, 0, SOIL_LOAD_RGBA);
    texPlayer.InternalFormat = GL_RGBA; texPlayer.ImageFormat = GL_RGBA;
    texPlayer.Generate(width, height, image);
    SOIL_free_image_data(image);
    // Level
    const unsigned char level[10][10] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
        { 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
        { 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, },
        { 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, },
    };
    // Vertex data
    GLfloat vertices[] = {
        // Position // Texture Coords
        0.0f, 1.0f, 0.0f, 1.0f, // Top Left
        0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
        1.0f, 1.0f, 1.0f, 1.0f, // Top Right
        1.0f, 0.0f, 1.0f, 0.0f  // Bottom Right 
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO); glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // Unbind VAO

    // Matrix setup
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)GAME_WIDTH, (GLfloat)GAME_HEIGHT, 0.0f, -1.0f, 1.0f);
    shaderSprite.SetMatrix4("projection", projection, true);
    shaderSprite.SetInteger("sprite", 0);
    

    // Initialize
    GameApplication::GetInstance()->Initialize(GAME_WIDTH, GAME_HEIGHT); // TODO: replace with current framebuffer size, not initial size

    //ResourceManager::GetInstance()->LoadLevel(GameApplication::GetInstance()->GetScene(), "levels/begin.lvl");

    // - game loop
    while (!glfwWindowShouldClose(window))
    {
        // - events
        glfwPollEvents();

        // - update game 
        GameApplication::GetInstance()->Update(0.031);

        //// - render game
        GameApplication::GetInstance()->Render();
        
        // - frame init
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // - render
        //shaderSprite.Use();
        //glBindVertexArray(VAO);
        //// background
        //texBackground.Bind(0);
        //glm::mat4 model; model = glm::scale(model, glm::vec3(GAME_WIDTH, GAME_HEIGHT, 1.0));
        //shaderSprite.SetMatrix4("model", model);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // player
        //texPlayer.Bind();
        //model = glm::mat4();
        //model = glm::translate(model, glm::vec3(150.0, 413.0, 0.0));
        //model = glm::scale(model, glm::vec3(65.0, 65.0, 1.0));
        //shaderSprite.SetMatrix4("model", model);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


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