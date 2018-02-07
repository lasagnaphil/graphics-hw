//
// Created by lasagnaphil on 2017-03-25.
//

#include "Node.h"
#include "Spatial.h"

Node::Node(const std::vector<Node*> children)
        : children(children) {
    spatialRepNode = reinterpret_cast<Spatial*>(findSpatial(this));
}

Node::~Node() {
    for(Node* child : children) {
        delete child;
    }
}

void Node::addChild(Node* child) {
    children.push_back(child);
    child->parent = this;
}

void Node::removeChild(Node* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    delete child;
}

Node* Node::findSpatial(Node* node) const {
    Spatial* spatial = dynamic_cast<Spatial*>(this->parent);
    if (spatial) return spatial;
    else return findSpatial(spatial);
}
