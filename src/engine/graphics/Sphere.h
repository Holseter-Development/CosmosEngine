#ifndef SPHERE_H
#define SPHERE_H

#include <vector>

class Sphere {
public:
    Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18);
    void Draw();

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    void buildVertices(float radius, int sectorCount, int stackCount);
    void setupMesh();
};

#endif