#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/mesh.h"
#include "../shader/shader.h"
class Pyramid {
  private:

    unsigned int m_Texture;
    Shader m_Shader;
    GLuint m_VAO;
  public:
    Pyramid(std::string shader);
    void LoadPyramidVAO();
    void Draw();

};