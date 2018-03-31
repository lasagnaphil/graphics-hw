#pragma once

#include <glad/glad.h>
#include <memory>
#include "Spatial.h"
#include "../Mesh.h"
#include "../Material.h"

class MeshNode : public Spatial {
public:
    MeshNode() = default;
    MeshNode(std::shared_ptr<Mesh> mesh, Shader shader);

    virtual void update(float dt) override;
    virtual void render() override;

    void setMesh(std::shared_ptr<Mesh> mesh);
    void setShader(Shader shader);

    std::shared_ptr<Mesh> mesh;
    Shader shader;
};

