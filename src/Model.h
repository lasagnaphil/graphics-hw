//
// Created by lasagnaphil on 2/7/18.
//

#ifndef GENGINE_MODEL_H
#define GENGINE_MODEL_H

#include "Shader.h"
#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "nodes/MeshNode.h"

class Model {
public:
    Model(const char *path) {
        loadModel(path);
    }

    void draw(Shader shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;

    std::vector<Texture> texturesLoaded;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};


#endif //GENGINE_MODEL_H
