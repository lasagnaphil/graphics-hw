//
// Created by lasagnaphil on 2/7/18.
//

#include "Model.h"

void Model::draw(Shader shader) {
    for (unsigned int i = 0; i < meshes.size(); ++i) {
        meshes[i].draw(shader);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    using std::vector;
    vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    vector<unsigned int> indices;
    indices.reserve(mesh->mNumFaces);
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.position = glm::vec3(
                mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z
        );

        vertex.normal = glm::vec3(
                mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z
        );

        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(
                    mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y
            );
        }
        else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto material = Material::create();
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *aMaterial = scene->mMaterials[mesh->mMaterialIndex];
        vector<Texture> diffuseMaps = loadMaterialTextures(
                aMaterial, aiTextureType_DIFFUSE
        );
        material->textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(
                aMaterial, aiTextureType_SPECULAR
        );
        material->textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }
    return Mesh(vertices, indices, material);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
    std::vector<Texture> textures;
    textures.reserve(mat->GetTextureCount(type));
    for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < texturesLoaded.size(); ++j) {
            if (std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Image image((directory + "/" + str.C_Str()).c_str());
            Texture texture = Texture::fromImage(image);
            if (type == aiTextureType_DIFFUSE) {
                texture.type = TextureType::Diffuse;
            } else if (type == aiTextureType_SPECULAR) {
                texture.type = TextureType::Specular;
            }
            texture.path = str.C_Str();
            textures.push_back(texture);
            texturesLoaded.push_back(texture);
        }
    }
    return textures;
}
