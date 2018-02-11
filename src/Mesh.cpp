//
// Created by lasagnaphil on 2/7/18.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    setupMesh();
}

void Mesh::draw(Shader shader) {
    using std::string;
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        TextureType type = textures[i].type;
        if (type == TextureType::Diffuse) {
            number = std::to_string(diffuseNr);
            diffuseNr++;
            shader.setFloat(string("material.texture_diffuse").append(number).c_str(), i);
        }
        else if (type == TextureType::Specular) {
            number = std::to_string(specularNr++);
            specularNr++;
            shader.setFloat(string("material.texture_specular").append(number).c_str(), i);
        }

        glBindTexture(GL_TEXTURE_2D, textures[i].getID());
    }
    glActiveTexture(GL_TEXTURE0);

    shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

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

static glm::vec3 cubeVertices[8] = {
        glm::vec3(-1, -1, -1),
        glm::vec3( 1, -1, -1),
        glm::vec3( 1,  1, -1),
        glm::vec3(-1,  1, -1),
        glm::vec3(-1, -1,  1),
        glm::vec3( 1, -1,  1),
        glm::vec3( 1,  1,  1),
        glm::vec3(-1,  1,  1)
};
static glm::vec2 cubeTexCoords[4] = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
};
static glm::vec3 cubeNormals[8] = {
        glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 0, -1),
        glm::vec3(-1, 0, 1),
        glm::vec3(0, 1, 0),
        glm::vec3(0, -1, 1),
};
static unsigned int cubeIndices[36] = {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
};
static unsigned int cubeTexIndices[6] = {
        0, 1, 3, 3, 1, 2
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

Mesh Mesh::createCube() {
    using std::vector;
    vector<Vertex> vertices(36);
    for (unsigned int i = 0; i < 36; ++i) {
        vertices[i].position = cubeVertices[cubeIndices[i]];
        vertices[i].normal = cubeNormals[cubeIndices[i / 6]];
        vertices[i].texCoords = cubeTexCoords[cubeTexIndices[i % 4]];
    }
    return Mesh(vertices, std::vector<unsigned int>(std::begin(cubeIndices), std::end(cubeIndices)), {});
}

Mesh Mesh::createSphere() {
    return Mesh({}, {}, {});
}

Mesh Mesh::createPlane() {
    return Mesh(std::vector<Vertex>(std::begin(planeVertices), std::end(planeVertices)),
                std::vector<unsigned int>(std::begin(planeIndices), std::end(planeIndices)), {});
}

Mesh* Mesh::createCubeDyn() {
    using std::vector;
    vector<Vertex> vertices(36);
    for (unsigned int i = 0; i < 36; ++i) {
        vertices[i].position = cubeVertices[cubeIndices[i]];
        vertices[i].normal = cubeNormals[cubeIndices[i / 6]];
        vertices[i].texCoords = cubeTexCoords[cubeTexIndices[i % 4]];
    }
    return new Mesh(vertices, std::vector<unsigned int>(std::begin(cubeIndices), std::end(cubeIndices)), {});
}

Mesh* Mesh::createSphereDyn() {
    return new Mesh(std::vector<Vertex>(), std::vector<unsigned int>(), std::vector<Texture>());
}

Mesh* Mesh::createPlaneDyn() {
    return new Mesh(std::vector<Vertex>(std::begin(planeVertices), std::end(planeVertices)),
                std::vector<unsigned int>(std::begin(planeIndices), std::end(planeIndices)), {});
}

