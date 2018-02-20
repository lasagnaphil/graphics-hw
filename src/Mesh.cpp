//
// Created by lasagnaphil on 2/7/18.
//

#include <glm/gtc/constants.hpp>
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures,
           GLenum drawMode,
           bool isIndexed)
        : vertices(std::move(vertices)),
          indices(std::move(indices)),
          textures(std::move(textures)),
          drawMode(drawMode),
          isIndexed(isIndexed)
{
    setupMesh();
}

void Mesh::draw(Shader shader) {
    using std::string;
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    shader.use();
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        TextureType type = textures[i].type;
        if (type == TextureType::Diffuse) {
            number = std::to_string(diffuseNr);
            diffuseNr++;
            shader.setInt(string("material.texture_diffuse").append(number).c_str(), i);
        }
        else if (type == TextureType::Specular) {
            number = std::to_string(specularNr);
            specularNr++;
            shader.setInt(string("material.texture_specular").append(number).c_str(), i);
        }

        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }

    glBindVertexArray(vao);
    if (isIndexed) {
        glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(drawMode, 0, vertices.size());
    }
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setMaterial(const Material& material) {
    textures.push_back(material.diffuse);
    textures.push_back(material.specular);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    if (isIndexed) {
        glGenBuffers(1, &ebo);
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    if (isIndexed) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

static Vertex cubeVertices[36] = {
        // positions          // normals           // texture coords
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f},
        {0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f},
        {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f},
        {0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f},

        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f},
        {0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f},
        {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f},
        {0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f},

        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f},
        {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f},
        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f},

        {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f},
        {0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f},
        {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
        {0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f},
        {0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f},
        {0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f},

        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f},
        {0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f},
        {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f},
        {0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f},
        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f},

        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f},
        {0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f},
        {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f},
        {0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f}
};

static unsigned int cubeIndices[36] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
};

static Vertex planeVertices[4] = {
        {0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f},
        {-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
        {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f}
};

static unsigned int planeIndices[6] = {
        0, 1, 3, 1, 2, 3
};

Mesh* Mesh::createCubeDyn() {
    return new Mesh(std::vector<Vertex>(std::begin(cubeVertices), std::end(cubeVertices)),
                    {}, {}, GL_TRIANGLES, false);
}

Mesh* Mesh::createSphereDyn() {
    return new Mesh(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<Texture>());
}

Mesh* Mesh::createPlaneDyn() {
    return new Mesh(std::vector<Vertex>(std::begin(planeVertices), std::end(planeVertices)),
                std::vector<unsigned int>(std::begin(planeIndices), std::end(planeIndices)), {});
}

Mesh* Mesh::createConeDyn(unsigned int numTriangles, float r, float h) {
    std::vector<Vertex> vertices;
    vertices.reserve(numTriangles * 3);
    float delta = glm::two_pi<float>() / numTriangles;
    for (unsigned int i = 0; i < numTriangles; ++i) {
        float theta = i * delta;
        float thetap = (i + 1) * delta;
        float thetam = (theta + thetap) / 2;
        float slopeLen = glm::sqrt(r*r + h*h);
        glm::vec3 normal(
            glm::cos(thetam) * h / slopeLen, r / slopeLen, -glm::sin(thetam) * h / slopeLen
        );
        vertices.push_back(Vertex({0.0f, h, 0.0f}, normal, {0.0f, 0.0f}));
        vertices.push_back(Vertex({r * glm::cos(theta), 0, -r * glm::sin(theta)}, normal, {1.0f, 0.0f}));
        vertices.push_back(Vertex({r * glm::cos(thetap), 0, -r * glm::sin(thetap)}, normal, {0.0f, 1.0f}));
    }
    return new Mesh(vertices, {}, {}, GL_TRIANGLES, false);
}

Mesh* Mesh::createCircleDyn(unsigned int numTriangles, float radius) {
    return createConeDyn(numTriangles, radius, 0.0f);
}

Mesh* Mesh::createCylinderDyn(unsigned int numQuads, float r, float h) {
    std::vector<Vertex> vertices;
    vertices.reserve(numQuads * 6);
    float delta = glm::two_pi<float>() / numQuads;
    for (unsigned int i = 0; i < numQuads; ++i) {
        float theta = i * delta;
        float thetap = (i + 1) * delta;
        float thetam = (theta + thetap) / 2;
        glm::vec3 normal(r * glm::cos(thetam), h, -r * glm::sin(thetam));
        vertices.push_back(Vertex({r * glm::cos(theta), 0, -r * glm::sin(theta)}, normal, {0.0f, 0.0f}));
        vertices.push_back(Vertex({r * glm::cos(theta), h, -r * glm::sin(theta)}, normal, {0.0f, 1.0f}));
        vertices.push_back(Vertex({r * glm::cos(thetap), h, -r * glm::sin(thetap)}, normal, {1.0f, 1.0f}));
        vertices.push_back(Vertex({r * glm::cos(theta), 0, -r * glm::sin(theta)}, normal, {0.0f, 0.0f}));
        vertices.push_back(Vertex({r * glm::cos(thetap), h, -r * glm::sin(thetap)}, normal, {1.0f, 1.0f}));
        vertices.push_back(Vertex({r * glm::cos(thetap), 0, -r * glm::sin(thetap)}, normal, {1.0f, 0.0f}));
    }
    return new Mesh(vertices, {}, {}, GL_TRIANGLES, false);
}

