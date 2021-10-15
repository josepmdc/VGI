// ---
// https://learnopengl.com/Model-Loading/Mesh
// ---
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "../shader/shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
    public:
        std::vector<Vertex>       m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture>      m_Textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader &shader);

    private:
        unsigned int VAO, VBO, EBO;

        void SetupMesh();
};
