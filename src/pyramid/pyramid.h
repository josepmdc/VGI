#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/mesh.h"

class Pyramid {
  private:

    unsigned int m_Texture;


    GLuint m_VAO;
  public:
    Pyramid();
    void LoadPyramidVAO();
    void Draw();

};