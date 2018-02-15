//
// Created by lasagnaphil on 2017-03-25.
//

#ifndef GENGINE_NODE_H
#define GENGINE_NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <algorithm>
#include <SDL2/SDL_events.h>

class Scene;

class Spatial;

class Node {
    friend class Scene;
public:
    Node(const std::vector<Node*> children = std::vector<Node*>(0));

    virtual ~Node();

    void addChild(Node* child);

    void removeChild(Node* child);

    std::vector<Node*> getChildren() { return children; }
    void setChildren(const std::vector<Node*>& children) { this->children = children; }

    virtual void update(float dt) {}
    virtual void render() {}
    virtual void processInput(SDL_Event& event) {}

protected:

    Scene* scene;

    std::vector<Node*> children;
    Node* parent;
    Spatial* spatialRepNode;

private:
    Node* findSpatial(Node* node);
};

#endif