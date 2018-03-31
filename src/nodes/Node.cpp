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

Node* Node::getChild(const std::string& name) {
    auto it = std::find_if(children.begin(), children.end(),
                        [&](Node* child){ return child->name == name; });
    if (it != children.end()) return *it;
    else return nullptr;
}

Node* Node::query(std::string path) {
    Node* node = this;

    // split string by delimiter "."
    size_t pos = 0;
    std::string token;
    while ((pos = path.find('.')) != std::string::npos) {
        token = path.substr(0, pos);
        node = node->getChild(token);
        if (!node) {
            return nullptr;
        }
        path.erase(0, pos + 1);
    }
    node = node->getChild(path);
    return node;
}
