//
// Created by lasagnaphil on 2017-03-25.
//

#ifndef GENGINE_NODE_H
#define GENGINE_NODE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <vector>
#include <algorithm>

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

protected:

    std::vector<Node*> children;
    Node* parent;
    Spatial* spatialRepNode;

private:
    Node* findSpatial(Node* node) const;
};

#endif
