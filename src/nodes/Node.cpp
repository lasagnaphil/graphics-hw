//
// Created by lasagnaphil on 2017-03-25.
//

#include "Node.h"
#include "Spatial.h"

Node::Node(const std::vector<Node*> children)
        : children(children) {
}

Node::~Node() {
    for(Node* child : children) {
        delete child;
    }
}

void Node::addChild(Node* child) {
    children.push_back(child);
    child->parent = this;
    child->scene = scene;
    Spatial* spatialChild = dynamic_cast<Spatial*>(child);
    if (spatialChild) {
        spatialRepNode->addSpatialChild(spatialChild);
        spatialChild->setSpatialParent(spatialRepNode);
    }
    else {
        child->spatialRepNode = spatialRepNode;
    }
}

void Node::removeChild(Node* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    delete child;
}

Node* Node::findSpatial(Node* node) {
    Spatial* spatial = dynamic_cast<Spatial*>(node);
    if (spatial) return this;
    else return findSpatial(this->parent);
}
