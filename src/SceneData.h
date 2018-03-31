//
// Created by lasagnaphil on 3/31/18.
//

#ifndef GENGINE_SCENEDATA_H
#define GENGINE_SCENEDATA_H

#include <yaml-cpp/yaml.h>
#include "glm_yaml_conv.h"

#include <string>
#include <unordered_map>

#include "Texture.h"
#include "Mesh.h"
#include "nodes/Node.h"
#include "nodes/LightNode.h"
#include "nodes/MeshNode.h"
#include "nodes/Camera.h"
#include "Scene.h"

class SceneData {
public:
    Scene* load(const std::string& filename);
    void setDefaultShader(Shader shader) { this->defaultShader = shader; }

    Camera* getCamera() { return camera; }

private:

    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
    std::unordered_map<std::string, std::shared_ptr<Material>> materials;
    std::unordered_map<std::string, Node*> nodes;

    Shader defaultShader;
    std::unique_ptr<Scene> scene;

    Camera* camera;

    void printMark(const YAML::Mark& mark);
    void loadError(const YAML::Mark& mark, const char* err);

    template <typename T>
    std::optional<T> loadField(const YAML::Node& data, const char* field) {
        if (data[field]) {
            return data[field].as<T>();
        }
        else {
            printMark(data.Mark());
            printf("missing field \"%s\"\n", field);
            return {};
        }
    }

    template <typename T>
    std::optional<T> loadFieldOpt(const YAML::Node& data, const char* field) {
        if (data[field]) {
            return data[field].as<T>();
        }
        else {
            return {};
        }
    }

    Node* loadNode(const YAML::Node& data);

    void loadChildren(Node* node, const YAML::Node& data);
};


#endif //GENGINE_SCENEDATA_H
