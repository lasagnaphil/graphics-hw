//
// Created by lasagnaphil on 3/31/18.
//

#include <yaml-cpp/yaml.h>
#include "SceneData.h"

Scene* SceneData::load(const std::string& filename) {
    using std::string;

    scene = std::make_unique<Scene>();

    YAML::Node data = YAML::LoadFile(filename);
    auto textureData = data["textures"];
    for (auto it = textureData.begin(); it != textureData.end(); ++it) {
        auto name = it->first.as<string>();
        if (auto color = loadFieldOpt<glm::vec4>(it->second, "color")) {
            textures[name] =
                Texture::fromColor(*color);
        }
        else if (auto color = loadFieldOpt<glm::vec4>(it->second, "colorInt")) {
            textures[name] =
                Texture::fromColor(*color / 255.f);
        }
        else {
            loadError(it->Mark(), "field color or colorInt missing");
        }
    }

    // Auto-load materials (using default specular texture)
    auto defaultSpecularTex = textures["defaultSpecular"];
    for (const auto& entry : textures) {
        materials[entry.first] = Material::create({entry.second, defaultSpecularTex}, 64.0f);
    }

    auto meshData = data["meshes"];
    for (auto it = meshData.begin(); it != meshData.end(); ++it) {
        std::string name = it->first.as<std::string>();
        if (auto type = loadField<string>(it->second, "type")) {
            if (type == "Cube") {
                meshes[name] = Mesh::createCube();
            }
            else if (type == "Circle") {
                int poly = it->second["poly"].as<int>();
                float radius = it->second["radius"].as<float>();
                meshes[name] = Mesh::createCircle(poly, radius);
            }
            else if (type == "Cylinder") {
                int poly = it->second["poly"].as<int>();
                float radius = it->second["radius"].as<float>();
                float height = it->second["height"].as<float>();
                meshes[name] = Mesh::createCylinder(poly, radius, height);
            }
            else {
                loadError(it->second["type"].Mark(), "unknown mesh type");
            }
        }
        if (auto matName = loadFieldOpt<string>(it->second, "mat")) {
            if (meshes.count(*matName)) {
                meshes[name]->setMaterial(materials[*matName]);
            }
            else {
                loadError(it->second["mat"].Mark(), "unknown mat type");
            }
        }
        else {
            meshes[name]->setMaterial(materials["default"]);
        }
    }


    auto nodes = data["nodes"];
    if (nodes) {
        loadChildren(scene->getRootNode(), nodes);
    }

    scene->addShader(defaultShader);

    return scene.get();
}

void SceneData::loadError(const YAML::Mark& mark, const char* err) {
    printMark(mark);
    printf(err);
    printf("\n");
    exit(1);
}

void SceneData::printMark(const YAML::Mark& mark) {
    printf("At line %d, column %d, pos %d:\n", mark.column, mark.line, mark.pos);
}

Node* SceneData::loadNode(const YAML::Node& data) {
    using std::string;

    Node* node;

    if (auto type = loadFieldOpt<string>(data, "type")) {
        if (type == "Node") { node = new Node(); }
        else if (type == "Spatial") { node = new Spatial(); }
        else if (type == "Camera") { node = new Camera(); camera = dynamic_cast<Camera*>(node); }
        else if (type == "MeshNode") { node = new MeshNode(); }
        else if (type == "LightNode") { node = new LightNode(); }
        else {
            // set default as MeshNode
            loadError(data["type"].Mark(), "invalid node type");
        }
    }
    else {
        node = new MeshNode();
    }

    if (auto name = loadField<string>(data, "name")) {
        node->name = *name;
    }

    auto spatial = dynamic_cast<Spatial*>(node);
    if (spatial) {
        if (auto pos = loadFieldOpt<glm::vec3>(data, "pos")) {
            spatial->setPosition(*pos);
        }
        if (auto rot = loadFieldOpt<glm::vec3>(data, "rot")) {
            auto rotation = glm::quat(glm::eulerAngleXYZ(glm::radians(rot->x), glm::radians(rot->y), glm::radians(rot->z)));
            spatial->setRotation(rotation);
        }
        if (auto scale = loadFieldOpt<glm::vec3>(data, "scale")) {
            spatial->setScale(*scale);
        }
    }

    auto meshNode = dynamic_cast<MeshNode*>(node);
    if (meshNode) {
        if (auto meshName = loadField<string>(data, "mesh")) {
            auto found = meshes.find(*meshName);
            if (found != meshes.end()) {
                meshNode->setMesh(meshes[*meshName]);
            }
            else {
                loadError(data["mesh"].Mark(), "mesh not found");
            }
        }
        meshNode->shader = defaultShader;
    }

    auto lightNode = dynamic_cast<LightNode*>(node);
    if (lightNode) {
        if (auto lightType = loadFieldOpt<string>(data, "lightType")) {
            if (*lightType == "Point") {
                lightNode->changeType(LightNode::Type::Point);
            }
            else if (*lightType == "Directional") {
                lightNode->changeType(LightNode::Type::Directional);
            }
            else if (*lightType == "Spotlight") {
                lightNode->changeType(LightNode::Type::Spotlight);
            }
            else {
                loadError(data["lightType"].Mark(), "invalid light type");
            }
        }
        if (auto ambient = loadFieldOpt<glm::vec3>(data, "ambientColor")) {
            lightNode->ambientColor = *ambient;
        }
        if (auto diffuse = loadFieldOpt<glm::vec3>(data, "diffuseColor")) {
            lightNode->diffuseColor = *diffuse;
        }
        if (auto specular = loadFieldOpt<glm::vec3>(data, "specularColor")) {
            lightNode->specularColor = *specular;
        }
    }

    if (data["children"]) {
        loadChildren(node, data["children"]);
    }
    return node;
}

void SceneData::loadChildren(Node* node, const YAML::Node& data) {
    using std::string;

    std::vector<Node*> children;
    for (auto it = data.begin(); it != data.end(); ++it) {
        Node* child = loadNode(*it);

        node->addChild(child);
    }
}

