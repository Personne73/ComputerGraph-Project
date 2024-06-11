#define GL_IGNORE_DEPRECATION

//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"

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

    // void Render()
    // { 
	// 	//glClearColor(1.f, 1.f, 0.f, 1.f);
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     uint32_t program = m_basicProgram.GetProgram();
    //     glUseProgram(program);

    //     double time = glfwGetTime();
    //     int timeLocation = glGetUniformLocation(program, "u_Time");
    //     glUniform1f(timeLocation, static_cast<float>(time));

	// 	const Vertex triangles[] =
	// 	{
	// 		{ { 0.0f, 0.5f } },    // sommet 0
	// 		{ { -0.5f, -0.5f } },  // sommet 1
	// 		{ { 0.5f, -0.5f } },   // sommet 2
	// 	};
	// 	//GLuint == uint32_t
	// 	//GLushort == uint16_t
	// 	const uint16_t indices[] = { 0, 1, 2 };

	// 	// {[x, y], [r, g, b]}
	// 	const int POSITION = glGetAttribLocation(program, "a_Position");
	// 	if (POSITION < 0)
	// 		std::cout << "erreur de programme";
	// 	glVertexAttribPointer(POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex)/*stride*/, &triangles);
	// 	glEnableVertexAttribArray(POSITION);

    //     //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indices);
    //     glDrawArrays(GL_TRIANGLES, 0, 3);
    // }
    void Render()
    {
        int width = 800;
        int height = 600;
        // etape a. A vous de recuperer/passer les variables width/height
        glViewport(0, 0, width, height);
        // etape b. Notez que glClearColor est un etat, donc persistant
        glClearColor(0.5f, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        // etape c. on specifie le shader program a utiliser
        auto basicProgram = m_basicProgram.GetProgram();
        glUseProgram(basicProgram);
        // etape d.
        static const float triangle[] = {
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 1.0f
        };

        int loc_position = glGetAttribLocation(basicProgram, "a_Position");
        glEnableVertexAttribArray(loc_position);
        glVertexAttribPointer(loc_position, 2, GL_FLOAT, false, sizeof(float) * 5, triangle);

        int loc_color = glGetAttribLocation(basicProgram, "a_Color");
        glEnableVertexAttribArray(loc_color);
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, false, sizeof(float) * 5, triangle+2);

        // etape e.
        // etape f. dessin de triangles dont la definition provient d’un tableau
        // le rendu s’effectue ici en prenant 3 sommets a partir du debut du tableau (0)
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // on suppose que la phase d’echange des buffers front et back
        // le « swap buffers » est effectuee juste apres
    }
};



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Exemple OpenGL 2.+", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

//    // ICI !
//    GLenum error = glewInit();
//    if (error != GLEW_OK) {
//        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
//    }

    Application app;
    app.Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {        
        /* Render here */
        app.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    app.Terminate();

    glfwTerminate();
    return 0;
}


// g++ -o exemple_opengl2 exemple_opengl2.cpp common/GLShader.cpp -framework OpenGL -lglfw
// ./exemple_opengl2