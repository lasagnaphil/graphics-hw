#pragma once

#include <glad/glad.h>
#include <memory>
#include "Spatial.h"
#include "../Mesh.h"
#include "../Material.h"

class MeshNode : public Spatial {
public:
    MeshNode() = default;
    MeshNode(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader);

    virtual void update(float dt) override;
    virtual void render() override;

    void setMesh(std::shared_ptr<Mesh> mesh);
    void setShader(std::shared_ptr<Shader> shader);

    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;

    struct BoundingBox {
        glm::vec2 leftTop;
        glm::vec2 rightBottom;
    };

    BoundingBox findBoundingBox(glm::mat4 projViewMatrix);

private:
    template <typename InputIter, typename OutputIter, typename F>
    void flat_transform(InputIter begin, InputIter end, OutputIter out, F fn) {
        for (; begin != end; ++begin) {
            auto y = fn(*begin);
            std::copy(std::begin(y), std::end(y), out);
        }
    }

    std::vector<glm::vec3> getAllVertices();
    std::vector<glm::vec3> meshToVertices();
};

