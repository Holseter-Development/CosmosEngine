// --- src/engine/graphics/Model.h ---
#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <vector>

class Model {
public:
    Model(std::string const &path);
    void Draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;

    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    // Updated function signature to include the scene
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, const aiScene* scene, aiTextureType type, std::string typeName);
};
#endif
