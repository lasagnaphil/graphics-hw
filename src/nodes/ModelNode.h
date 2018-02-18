//
// Created by lasagnaphil on 2/18/18.
//

#ifndef GENGINE_MODELNODE_H
#define GENGINE_MODELNODE_H


#include "Spatial.h"
#include "../Model.h"

class ModelNode : public Spatial {
public:
    ModelNode(std::shared_ptr<Model> model, Shader shader)
            : model(std::move(model)), shader(shader) {}

    virtual void render() override;

private:
    std::shared_ptr<Model> model;
    Shader shader;
};


#endif //GENGINE_MODELNODE_H
