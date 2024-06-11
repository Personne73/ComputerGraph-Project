#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#include <cmath>
#include <iostream>

// Variables globales pour stocker les états de la caméra
float radius = 10.0f;
float azimuth = 0.0f; // rotation autour de l'axe vertical (y)
float elevation = 0.0f; // rotation autour de l'axe horizontal (x)
double lastX, lastY;
bool firstMouse = true;

// Callback de la souris pour gérer la rotation de la caméra
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // inversé car les coordonnées y vont de bas en haut
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // Ajuster selon les besoins
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    azimuth += xoffset;
    elevation += yoffset;

    // Limiter les angles
    if (elevation > 89.0f) elevation = 89.0f;
    if (elevation < -89.0f) elevation = -89.0f;
}

// Callback de la molette pour gérer le zoom
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    radius -= yoffset;
    if (radius < 1.0f) radius = 1.0f;
}

// Calculer la position de la caméra en utilisant les coordonnées sphériques
void computeCameraPosition(float& x, float& y, float& z) {
    x = radius * cosf(elevation * M_PI / 180.0f) * cosf(azimuth * M_PI / 180.0f);
    y = radius * sinf(elevation * M_PI / 180.0f);
    z = radius * cosf(elevation * M_PI / 180.0f) * sinf(azimuth * M_PI / 180.0f);
}

void drawCube() {
    glBegin(GL_QUADS);
    // Face avant
    glColor3f(1.0f, 0.0f, 0.0f); // rouge
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    // Face arrière
    glColor3f(0.0f, 1.0f, 0.0f); // vert
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    // Face gauche
    glColor3f(0.0f, 0.0f, 1.0f); // bleu
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    // Face droite
    glColor3f(1.0f, 1.0f, 0.0f); // jaune
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    // Face supérieure
    glColor3f(1.0f, 0.0f, 1.0f); // magenta
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    // Face inférieure
    glColor3f(0.0f, 1.0f, 1.0f); // cyan
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Camera Orbitale", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float cameraX, cameraY, cameraZ;
        computeCameraPosition(cameraX, cameraY, cameraZ);

        // Afficher les coordonnées de la caméra dans le terminal
        std::cout << "Camera Position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")\n";

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        // Dessinez votre scène ici
        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
