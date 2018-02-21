//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_MESH_H
#define GENGINE_MESH_H

#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    Vertex() {}
    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords)
            : position(position), normal(normal), texCoords(texCoords) {}
    Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
            : position(x, y, z), normal(nx, ny, nz), texCoords(u, v) {}
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::shared_ptr<Material> material;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::shared_ptr<Material> material,
         GLenum drawMode = GL_TRIANGLES,
         bool isIndexed = true);

    void draw(Shader shader);

    void setMaterial(std::shared_ptr<Material> material) {
        this->material = std::move(material);
    }

    static Mesh* createCubeDyn();
    static Mesh* createSphereDyn();
    static Mesh* createPlaneDyn();
    static Mesh* createConeDyn(unsigned int numTriangles, float radius, float height);
    static Mesh* createCircleDyn(unsigned int numTriangles, float radius);
    static Mesh* createCylinderDyn(unsigned int numQuads, float radius, float height);

private:
    unsigned int vao, vbo, ebo;

    GLenum drawMode;
    bool isIndexed;

    void setupMesh();

};


#endif //GENGINE_MESH_H
