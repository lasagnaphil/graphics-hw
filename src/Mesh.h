//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_MESH_H
#define GENGINE_MESH_H

#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex() {}
    Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
            : position(x, y, z), normal(nx, ny, nz), texCoords(u, v) {}
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);

    void draw(Shader shader);

    static Mesh createCube();
    static Mesh createSphere();
    static Mesh createPlane();

    static Mesh* createCubeDyn();
    static Mesh* createSphereDyn();
    static Mesh* createPlaneDyn();

private:
    unsigned int vao, vbo, ebo;

    void setupMesh();

};


#endif //GENGINE_MESH_H
