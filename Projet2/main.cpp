#define GL_IGNORE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
#include "common/tiny_obj_loader.cc"

#define INTERVAL 15

// Variables globales pour stocker les états de la caméra
double POS_X, POS_Y;

// Variables globales pour la lumière
GLfloat light_pos[] = {-10.0f, 10.0f, 100.00f, 1.0f};

// Variables globales pour la caméra
float pos_x = 0.0f, pos_y = 0.0f, pos_z = -20.0f;
float angle_x = 30.0f, angle_y = 0.0f;

// Variables globales pour la souris
int x_old = 0, y_old = 0;
int current_scroll = 0;
float zoom_per_scroll = 1.0f;

// Variables globales pour le clavier
bool is_holding_mouse = false;
bool is_updated = false;

// struct vec2 {
//     float x, y;
//     vec2() : x(0), y(0) {}
//     vec2(float x, float y) : x(x), y(y) {}
// };

// struct vec3 {
//     float x, y, z;
//     vec3() : x(0), y(0), z(0) {}
//     vec3(float x, float y, float z) : x(x), y(y), z(z) {}
// };

// struct Vertex {
//     vec3 position;
//     vec3 normal;
//     vec2 texCoord;
// };

// struct Material {
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
//     float shininess;
// };

// struct Mesh {
//     std::vector<Vertex> vertices;
//     std::vector<unsigned int> indices;
//     std::vector<Material> materials;
//     Material defaultMaterial;
//     GLuint vbo;
//     GLuint ebo;
// };

struct Application {
    // GLShader m_basicProgram;
    // Mesh m_mesh;

    void Initialize() {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        // GLfloat light_pos[] = {-1.0f, 10.0f, 100.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(20.0, 1.0, 1.0, 2000.0);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);

        // m_basicProgram.LoadVertexShader("basic.vs.glsl");
        // m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        // m_basicProgram.Create();
    }

    void Terminate() {
        // m_basicProgram.Destroy();
    }

    void drawCube(){
        glBegin(GL_QUADS);

        // Face avant
        glColor3f(1.0f, 0.0f, 0.0f); // Rouge
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);

        // Face arrière
        glColor3f(0.0f, 1.0f, 0.0f); // Vert
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);

        // Face gauche
        glColor3f(0.0f, 0.0f, 1.0f); // Bleu
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);

        // Face droite
        glColor3f(1.0f, 1.0f, 0.0f); // Jaune
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);

        // Face supérieure
        glColor3f(1.0f, 0.0f, 1.0f); // Magenta
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f,  1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);

        // Face inférieure
        glColor3f(0.0f, 1.0f, 1.0f); // Cyan
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);

        glEnd();
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        // glTranslatef(0.0f, -30.0f, -500.0f);
        // glRotatef(30.0f, 1.0f, -1.0f, 0.0f);
        glTranslatef(pos_x, pos_y, pos_z);
        glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
        glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
        drawCube();
    }

    void Render() {
        // glUseProgram(m_basicProgram.GetProgram());
        // glClearColor(0.f, 1.f, 0.f, 1.f);
        display();
    }
};

// void timer(int value) {
//     if (is_updated) {
//         is_updated = false;
//         glutPostRedisplay();
//     }
//     glutTimerFunc(INTERVAL, timer, 0);
// }

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &POS_X, &POS_Y);
        x_old = POS_X;
        y_old = POS_Y;
        is_holding_mouse = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        is_holding_mouse = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (yoffset > 0 && current_scroll > 0) {
        current_scroll--;
        pos_z += zoom_per_scroll;
    } else if (yoffset < 0 && current_scroll < 50) {
        current_scroll++;
        pos_z -= zoom_per_scroll;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        if (is_holding_mouse) {
            angle_y += (xpos - x_old);
            x_old = xpos;
            if (angle_y > 360.0f)
                angle_y -= 360.0f;
            else if (angle_y < 0.0f)
                angle_y += 360.0f;

            angle_x += (ypos - y_old);
            y_old = ypos;
            if (angle_x > 90.0f)
                angle_x = 90.0f;
            else if (angle_x < -90.0f)
                angle_x = -90.0f;
        }
    }

int main(void) {
    int width = 1200;
    int height = 800;
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(width, height, "Projet - OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    glEnable(GL_DEPTH_TEST); // Enable depth testing for correct rendering

    Application app;
    glfwSetWindowUserPointer(window, &app);
    app.Initialize();

    // Set GLFW callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    while (!glfwWindowShouldClose(window)) {        
        app.Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    app.Terminate();
    glfwTerminate();
    return 0;
}

// Compilation:
// g++ -o main main.cpp common/GLShader.cpp -framework OpenGL -lglfw
// Exécution:
// ./main