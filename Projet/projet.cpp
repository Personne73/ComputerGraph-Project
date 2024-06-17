#define GL_IGNORE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
#include "common/tiny_obj_loader.cc"

struct vec2 { float x, y; };
struct vec3 { float x, y, z; };

struct Vertex {
    vec3 position;  // Position du sommet dans l'espace 3D
    vec3 normal;    // Vecteur normal au sommet (utile pour l'éclairage)
    vec2 texCoord;  // Coordonnées de texture (pour appliquer des textures)
};

struct Mesh {
    std::vector<Vertex> vertices;  // Liste des sommets du maillage
    std::vector<unsigned int> indices;  // Liste des indices des sommets pour définir les triangles
    GLuint vao;  // Vertex Array Object
    GLuint vbo;  // Vertex Buffer Object
    GLuint ebo;  // Element Buffer Object (ou Index Buffer Object)
};


struct Application
{
    GLShader m_basicProgram;
    Mesh m_mesh;

    void Initialize()
    {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();

        // LoadObj("file/IronMan.obj");
        LoadOBJ("file/IronMan.obj", m_mesh);
        SetupMesh(m_mesh);
    }

    void Terminate() {
        glDeleteVertexArrays(1, &m_mesh.vao);
        glDeleteBuffers(1, &m_mesh.vbo);
        glDeleteBuffers(1, &m_mesh.ebo);
        m_basicProgram.Destroy();
    }

    // Load the obj file
    void LoadOBJ(const std::string& filename, Mesh& mesh) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
        if (!warn.empty()) std::cout << "WARN : " << warn << std::endl;
        if (!err.empty()) std::cerr << "ERR : " << err << std::endl;
        if (!ret) {
            std::cerr << "Failed to load OBJ file : " << filename << std::endl;
            return;
        }

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};
                vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
                vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
                vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

                if (!attrib.normals.empty()) {
                    vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
                    vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
                    vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
                }

                if (!attrib.texcoords.empty()) {
                    vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
                    vertex.texCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];
                }


                mesh.vertices.push_back(vertex);
                mesh.indices.push_back(mesh.indices.size());
            }
        }
    }

    void SetupMesh(Mesh& mesh) {
        glGenVertexArrays(1, &mesh.vao);
        glBindVertexArray(mesh.vao);

        glGenBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
        // q : why unbind VAO ?
        // a : to prevent bugs, it's a good practice to unbind the VAO after setting it up
        // q : why do we don't unbind VBO and EBO ?
        // a : because the VAO stores the VBO and EBO, so unbinding the VAO will unbind the VBO and EBO
    }

    void Render()
    {
        glClearColor(0.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto basicProgram = m_basicProgram.GetProgram();
        glUseProgram(basicProgram);

        glBindVertexArray(m_mesh.vao);
        glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

int main(void) {
    int width = 800;
    int height = 600;
    GLFWwindow* window;

    if (!glfwInit()) return -1;

    window = glfwCreateWindow(width, height, "Projet - OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    Application app;
    app.Initialize();

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
// g++ -o projet projet.cpp common/GLShader.cpp common/tiny_obj_loader.cc -framework OpenGL -lglfw
// Exécution:
// ./projet
