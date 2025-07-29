#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    // Mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;

    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    // Render the mesh
    void Draw(Shader &shader);

private:
    // Render data 
    unsigned int VAO, VBO, EBO;

    // Initializes all the buffer objects/arrays
    void setupMesh();
};
#endif