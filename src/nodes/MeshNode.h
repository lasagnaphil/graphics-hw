#pragma once

#include <glad/glad.h>
#include <memory>
#include "Spatial.h"
#include "../Mesh.h"

class MeshNode : public Spatial {
public:
    MeshNode(std::shared_ptr<Mesh> mesh, Shader shader);

    virtual void update(float dt) override;
    virtual void render() override;

    MeshNode& setShader(Shader shader);

protected:

    std::shared_ptr<Mesh> mesh;
    Shader shader;

    GLint uniWorldTransform;
};

