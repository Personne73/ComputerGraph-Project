#define GL_IGNORE_DEPRECATION

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "common/GLShader.h"
#include "common/tiny_obj_loader.cc"

// struct vec2 { float x, y; };
//struct vec3 { float x, y, z; };

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
    vec3 position;  // Position du sommet dans l'espace 3D
    vec3 normal;    // Vecteur normal au sommet (utile pour l'éclairage)
    vec2 texCoord;  // Coordonnées de texture (pour appliquer des textures)
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess; // Brillance
};

struct Mesh {
    std::vector<Vertex> vertices;  // Liste des sommets du maillage
    std::vector<unsigned int> indices;  // Liste des indices des sommets pour définir les triangles
    std::vector<Material> materials;  // Matériau du maillage
    Material defaultMaterial;  // Matériau par défaut
    GLuint vao;  // Vertex Array Object
    GLuint vbo;  // Vertex Buffer Object
    GLuint ebo;  // Element Buffer Object (ou Index Buffer Object)
    std::vector<GLuint> vbos;
    std::vector<GLuint> vaos;
};

struct Application
{
    GLShader m_basicProgram;
    Mesh m_mesh;
    // std::vector<Material> m_materials;

    void Initialize()
    {
        m_basicProgram.LoadVertexShader("basic.vs.glsl");
        m_basicProgram.LoadFragmentShader("basic.fs.glsl");
        m_basicProgram.Create();

        //LoadOBJ("file/apple.obj", m_mesh);
        if (LoadObject("file/apple.obj", m_mesh)) {
            createVAOandVBO(m_mesh);
            std::cout << "Loaded OBJ file" << std::endl;
        }

        // if (!LoadObj("file/apple.obj", m_mesh, m_materials)) {
        //     std::cerr << "Failed to load OBJ file" << std::endl;
        //     return;
        // } else {
        //     std::cout << "Loaded OBJ file" << std::endl;
        // }
    }

    void Terminate() {
        glDeleteVertexArrays(1, &m_mesh.vao);
        glDeleteBuffers(1, &m_mesh.vbo);
        glDeleteBuffers(1, &m_mesh.ebo);
        m_basicProgram.Destroy();
    }

    // Load the obj file
    // bool LoadOBJ(const std::string& filename, Mesh& mesh) {
    //     tinyobj::attrib_t attrib;
    //     std::vector<tinyobj::shape_t> shapes;
    //     std::vector<tinyobj::material_t> materials;
    //     std::string warn, err;

    //     bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
    //     if (!warn.empty()) {
    //         std::cout << "WARN : " << warn << std::endl;
    //     }
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
    //             // mesh.material.ambient = vec3(0.1f, 0.1f, 0.1f);
    //             // mesh.material.diffuse = vec3(0.6f, 0.6f, 0.6f);
    //             // mesh.material.specular = vec3(0.3f, 0.3f, 0.3f);
    //             // mesh.material.shininess = 32.0f;
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

    //     // SetupMesh(m_mesh);
    //     return true;
    // }


    // Load obj using the code example from github.com/tinyobjloader/tinyobjloader
    bool LoadObject(const std::string& inputfile, Mesh& mesh){
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = ""; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {

            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                    Vertex vertex;
                    vertex.position = vec3(vx, vy, vz);

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                        tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                        tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                        vertex.normal = vec3(nx, ny, nz);
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                        tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                        vertex.texCoord = vec2(tx, ty);
                    }

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];

                    mesh.vertices.push_back(vertex);
                    mesh.indices.push_back(index_offset + v);
                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
            // GLuint vao, vbo[3];
            // glGenVertexArrays(1, &vao);
            // glGenBuffers(3, vbo);

            // glBindVertexArray(vao);

            // // Vertex positions
            // glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
            // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            // glEnableVertexAttribArray(0);

            // // Vertex normals
            // if (!normals.empty()) {
            //     glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            //     glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
            //     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            //     glEnableVertexAttribArray(1);
            // }

            // // Vertex texture coordinates
            // if (!texcoords.empty()) {
            //     glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
            //     glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(float), texcoords.data(), GL_STATIC_DRAW);
            //     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
            //     glEnableVertexAttribArray(2);
            // }

            // glBindBuffer(GL_ARRAY_BUFFER, 0);
            // glBindVertexArray(0);

            // mesh.vaos.push_back(vao);
            // mesh.vbos.push_back(vbo[0]);
            // mesh.vbos.push_back(vbo[1]);
            // mesh.vbos.push_back(vbo[2]);
        }

        // Create default material
        Material defaultMaterial;
        defaultMaterial.ambient = vec3(0.1f, 0.1f, 0.1f);
        defaultMaterial.diffuse = vec3(0.6f, 0.6f, 0.6f);
        defaultMaterial.specular = vec3(0.2f, 0.2f, 0.2f);
        defaultMaterial.shininess = 32.0f;
        mesh.defaultMaterial = defaultMaterial;


        return true;
    }

  void createVAOandVBO(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    if (!mesh.indices.empty()) {
        glGenBuffers(1, &mesh.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);
    }

    glBindVertexArray(0); // Délier le VAO pour éviter les modifications accidentelles
}


    // bool LoadObj(const char* filename, Mesh& mesh, std::vector<Material>& materials) {
    //     tinyobj::ObjReader reader;

    //     if (!reader.ParseFromFile(filename)) {
    //         if (!reader.Error().empty()) {
    //             std::cerr << "TinyObjReader: " << reader.Error() << std::endl;
    //         }
    //         return false;
    //     }

    //     if (!reader.Warning().empty()) {
    //         std::cout << "TinyObjReader: " << reader.Warning() << std::endl;
    //     }

    //     auto& attrib = reader.GetAttrib();
    //     auto& shapes = reader.GetShapes();
    //     auto& objMaterials = reader.GetMaterials();

    //     // Charger les matériaux
    //     for (const auto& material : objMaterials) {
    //         Material mat;
    //         mat.ambient = vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
    //         mat.diffuse = vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
    //         mat.specular = vec3(material.specular[0], material.specular[1], material.specular[2]);
    //         mat.shininess = material.shininess;
    //         materials.push_back(mat);
    //     }

    //     // Traiter les shapes pour extraire les données des vertex, incluant les IDs des matériaux
    //     for (size_t s = 0; s < shapes.size(); ++s) {
    //         const tinyobj::shape_t& shape = shapes[s];
    //         size_t index_offset = 0;

    //         for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
    //             int fv = shape.mesh.num_face_vertices[f];

    //             // Assume triangle faces
    //             for (size_t v = 0; v < fv; v++) {
    //                 tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
    //                 Vertex vertex;

    //                 vertex.position = vec3(
    //                     attrib.vertices[3 * idx.vertex_index + 0],
    //                     attrib.vertices[3 * idx.vertex_index + 1],
    //                     attrib.vertices[3 * idx.vertex_index + 2]);

    //                 if (idx.normal_index >= 0) {
    //                     vertex.normal = vec3(
    //                         attrib.normals[3 * idx.normal_index + 0],
    //                         attrib.normals[3 * idx.normal_index + 1],
    //                         attrib.normals[3 * idx.normal_index + 2]);
    //                 }

    //                 if (idx.texcoord_index >= 0) {
    //                     vertex.texCoord = vec2(
    //                         attrib.texcoords[2 * idx.texcoord_index + 0],
    //                         attrib.texcoords[2 * idx.texcoord_index + 1]);
    //                 }

    //                 mesh.vertices.push_back(vertex);
    //                 mesh.indices.push_back(index_offset + v);
    //             }

    //             // Get material ID and use it as needed
    //             int matID = shape.mesh.material_ids[f];
    //             if (matID >= 0 && matID < materials.size()) {
    //                 mesh.material = materials[matID];
    //             }

    //             index_offset += fv;
    //         }
    //     }

    //     SetupMesh(mesh);
    //     return true;
    // }

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

    // void SetMaterialUniforms(GLuint shaderProgram, const Material& mat) {
    //     glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), mat.ambient.x, mat.ambient.y, mat.ambient.z);
    //     glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), mat.diffuse.x, mat.diffuse.y, mat.diffuse.z);
    //     glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), mat.specular.x, mat.specular.y, mat.specular.z);
    //     glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), mat.shininess);
    // }

    void Render()
    {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto basicProgram = m_basicProgram.GetProgram();
        glUseProgram(basicProgram);

        glBindVertexArray(m_mesh.vao);

        for (size_t i = 0; i < m_mesh.materials.size(); ++i) {
            Material& material = m_mesh.materials[i];
            if (material.ambient.x < 0) {
                material = m_mesh.defaultMaterial;
            }

            GLint ambientLoc = glGetUniformLocation(basicProgram, "material.ambient");
            GLint diffuseLoc = glGetUniformLocation(basicProgram, "material.diffuse");
            GLint specularLoc = glGetUniformLocation(basicProgram, "material.specular");
            GLint shininessLoc = glGetUniformLocation(basicProgram, "material.shininess");

            glUniform3fv(ambientLoc, 1, &material.ambient.x);
            glUniform3fv(diffuseLoc, 1, &material.diffuse.x);
            glUniform3fv(specularLoc, 1, &material.specular.x);
            glUniform1f(shininessLoc, material.shininess);

            glDrawElements(GL_TRIANGLES, m_mesh.indices.size(), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);
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