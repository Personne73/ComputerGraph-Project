#define GL_IGNORE_DEPRECATION

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
#include "common/tiny_obj_loader.cc"

struct vec2 { float x, y; };
struct vec3 { float x, y, z; };
struct vec4 { float x, y, z, w; };

struct Vertex
{
    vec2 position;
};

struct Application
{
    GLShader m_basicProgram;

    void Initialize()
    {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();
    }

    void Terminate()
    {
        m_basicProgram.Destroy();
    }

    void LoadObj()
    {
        
    }

    void Render()
    {
        int width = 800;
        int height = 600;
        glViewport(0, 0, width, height);
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto basicProgram = m_basicProgram.GetProgram();
        glUseProgram(basicProgram);

        static const float triangle[] = {
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        int loc_position = glGetAttribLocation(basicProgram, "a_Position");
        glEnableVertexAttribArray(loc_position);
        glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, triangle);

        int loc_color = glGetAttribLocation(basicProgram, "a_Color");
        glEnableVertexAttribArray(loc_color);
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, triangle + 2);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};

int main(void)
{
    int width = 800;
    int height = 600;

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(width, height, "Projet - OpenGL 2", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    Application app;
    app.Initialize();

    while (!glfwWindowShouldClose(window))
    {        
        app.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.Terminate();
    glfwTerminate();
    return 0;
}

// Compilation:
// g++ -o projet projet.cpp common/GLShader.cpp -framework OpenGL -lglfw
// Exécution:
// ./projet
