#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../mesh/mesh.h"

class Model {
    public:
        Model(std::string const& path) { LoadModel(path); }
        void Draw(Shader &shader);	

    private:
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;
        bool gammaCorrection;

        void LoadModel(std::string const& path);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
        void ProcessNode(aiNode *node, const aiScene *scene);
        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
};
