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

struct Triangle {
    Vertex vertices[3];
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    std::vector<unsigned int> indices;
    std::vector<Material> materials;
    Material defaultMaterial;
    GLuint vbo;
    GLuint ebo;
};

struct Application {
    GLShader m_basicProgram;
    Mesh m_mesh;

    // Variables de la caméra orbitale
    float radius = 10.0f;
    float azimuth = 0.0f;
    float elevation = 0.0f;
    double lastX, lastY;
    bool firstMouse = true;

    void Initialize() {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();

        if (LoadObject("file/Car.obj", m_mesh)) {
            std::cout << "Loaded OBJ file" << std::endl;
        }
    }

    void Terminate() {
        glDeleteBuffers(1, &m_mesh.vbo);
        glDeleteBuffers(1, &m_mesh.ebo);
        m_basicProgram.Destroy();
    }

    // bool LoadObject(const std::string& filename, Mesh& mesh) {
    //     tinyobj::attrib_t attrib;
    //     std::vector<tinyobj::shape_t> shapes;
    //     std::vector<tinyobj::material_t> materials;
    //     std::string warn, err;

    //     bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
    //     if (!warn.empty()) std::cout << "WARN : " << warn << std::endl;
    //     if (!err.empty()) std::cerr << "ERR : " << err << std::endl;
    //     if (!ret) {
    //         std::cerr << "Failed to load OBJ file : " << filename << std::endl;
    //         return false;
    //     }

    //     Material defaultMaterial = {};
    //     defaultMaterial.ambient = vec3(0.2f, 0.2f, 0.2f);
    //     defaultMaterial.diffuse = vec3(0.8f, 0.8f, 0.8f);
    //     defaultMaterial.specular = vec3(1.0f, 1.0f, 1.0f);
    //     defaultMaterial.shininess = 32.0f;
    //     mesh.defaultMaterial = defaultMaterial;

    //     for (const auto& shape : shapes) {
    //         for (const auto& index : shape.mesh.indices) {
    //             Vertex vertex = {};
    //             vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
    //             vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
    //             vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

    //             if (!attrib.normals.empty()) {
    //                 vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
    //                 vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
    //                 vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
    //             }

    //             if (!attrib.texcoords.empty()) {
    //                 vertex.texCoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
    //                 vertex.texCoord.y = attrib.texcoords[2 * index.texcoord_index + 1];
    //             }

    //             mesh.vertices.push_back(vertex);
    //             mesh.indices.push_back(mesh.indices.size());
    //         }
    //     }

    //     std::cout << "Vertices loaded : " << mesh.vertices.size() << std::endl;
    //     std::cout << "Indices loaded : " << mesh.indices.size() << std::endl;

    //     if (!mesh.vertices.empty()) {
    //         SetupMesh(mesh);
    //     } else {
    //         std::cerr << "No vertices loaded for " << filename << std::endl;
    //         return false;
    //     }

    //     return true;
    // }

    bool LoadObject(const std::string& inputfile, Mesh& mesh) {
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = ""; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        return false;
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    std::vector<vec3> vertices;
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        vertices.emplace_back(
            attrib.vertices[i],
            attrib.vertices[i + 1],
            attrib.vertices[i + 2]);
    }

    std::vector<vec3> normals;
    for (size_t i = 0; i < attrib.normals.size(); i += 3) {
        normals.emplace_back(
            attrib.normals[i],
            attrib.normals[i + 1],
            attrib.normals[i + 2]);
    }

    std::vector<vec2> texcoords;
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2) {
        texcoords.emplace_back(
            attrib.texcoords[i],
            attrib.texcoords[i + 1]);
    }

    // Loop over shapes
    for (const auto& shape : shapes) {
        const auto& indices = shape.mesh.indices;
        for (size_t i = 0; i < indices.size(); i += 3) {
            Triangle triangle;
            for (int j = 0; j < 3; ++j) {
                int vertex_index = indices[i + j].vertex_index;
                int normal_index = indices[i + j].normal_index;
                int texcoord_index = indices[i + j].texcoord_index;

                triangle.vertices[j].position = vertices[vertex_index];
                if (normal_index >= 0) {
                    triangle.vertices[j].normal = normals[normal_index];
                }
                if (texcoord_index >= 0) {
                    triangle.vertices[j].texCoord = texcoords[texcoord_index];
                }
            }
            mesh.triangles.push_back(triangle);
        }
    }

    return true;
}

void drawMesh(const Mesh& mesh) {
    glBegin(GL_TRIANGLES);
    for (const auto& triangle : mesh.triangles) {
        for (const auto& vertex : triangle.vertices) {
            glNormal3f(vertex.normal.x, vertex.normal.y, vertex.normal.z);
            glTexCoord2f(vertex.texCoord.x, vertex.texCoord.y);
            glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
        }
    }
    glEnd();
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

        float cameraX, cameraY, cameraZ;
        computeCameraPosition(cameraX, cameraY, cameraZ);

        // Imprimer les coordonnées de la caméra
        std::cout << "Camera Position: (" << cameraX << ", " << cameraY << ", " << cameraZ << ")\n";

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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

    void computeCameraPosition(float& x, float& y, float& z) {
        x = radius * cosf(elevation * M_PI / 180.0f) * cosf(azimuth * M_PI / 180.0f);
        y = radius * sinf(elevation * M_PI / 180.0f);
        z = radius * cosf(elevation * M_PI / 180.0f) * sinf(azimuth * M_PI / 180.0f);
    }
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    if (app->firstMouse) {
        app->lastX = xpos;
        app->lastY = ypos;
        app->firstMouse = false;
    }

    float xoffset = xpos - app->lastX;
    float yoffset = app->lastY - ypos; // inversé car les coordonnées y vont de bas en haut
    app->lastX = xpos;
    app->lastY = ypos;

    float sensitivity = 0.1f; // Ajuster selon les besoins
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    app->azimuth += xoffset;
    app->elevation += yoffset;

    // Limiter les angles
    if (app->elevation > 89.0f) app->elevation = 89.0f;
    if (app->elevation < -89.0f) app->elevation = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = (Application*)glfwGetWindowUserPointer(window);
    app->radius -= yoffset;
    if (app->radius < 1.0f) app->radius = 1.0f;
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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glEnable(GL_DEPTH_TEST); // Enable depth testing for correct rendering

    Application app;
    glfwSetWindowUserPointer(window, &app);
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
// g++ -o projet projet.cpp common/GLShader.cpp -framework OpenGL -lglfw
// Exécution:
// ./projet