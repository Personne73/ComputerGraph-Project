#define GL_IGNORE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
#include "common/tiny_obj_loader.cc"

struct vec2 {
    float x, y;
    vec2() : x(0), y(0) {}
    vec2(float x, float y) : x(x), y(y) {}
};

struct vec3 {
    float x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Material> materials;
    Material defaultMaterial;
    GLuint vbo;
    GLuint ebo;
};

struct Application {
    GLShader m_basicProgram;
    Mesh m_mesh;

    void Initialize() {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();

        if (LoadObject("file/apple.obj", m_mesh)) {
            std::cout << "Loaded OBJ file" << std::endl;
        }
    }

    void Terminate() {
        glDeleteBuffers(1, &m_mesh.vbo);
        glDeleteBuffers(1, &m_mesh.ebo);
        m_basicProgram.Destroy();
    }

    bool LoadObject(const std::string& filename, Mesh& mesh) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
        if (!warn.empty()) std::cout << "WARN : " << warn << std::endl;
        if (!err.empty()) std::cerr << "ERR : " << err << std::endl;
        if (!ret) {
            std::cerr << "Failed to load OBJ file : " << filename << std::endl;
            return false;
        }

        Material defaultMaterial = {};
        defaultMaterial.ambient = vec3(0.2f, 0.2f, 0.2f);
        defaultMaterial.diffuse = vec3(0.8f, 0.8f, 0.8f);
        defaultMaterial.specular = vec3(1.0f, 1.0f, 1.0f);
        defaultMaterial.shininess = 32.0f;
        mesh.defaultMaterial = defaultMaterial;

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

        std::cout << "Vertices loaded : " << mesh.vertices.size() << std::endl;
        std::cout << "Indices loaded : " << mesh.indices.size() << std::endl;

        if (!mesh.vertices.empty()) {
            SetupMesh(mesh);
        } else {
            std::cerr << "No vertices loaded for " << filename << std::endl;
            return false;
        }

        return true;
    }

    void SetupMesh(Mesh& mesh) {
        glGenBuffers(1, &mesh.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);
    }

    void Render() {
        glClearColor(0.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto basicProgram = m_basicProgram.GetProgram();
        glUseProgram(basicProgram);

        glBindBuffer(GL_ARRAY_BUFFER, m_mesh.vbo);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));

        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_mesh.ebo);
        glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
};

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
