#include <string>
#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>
#include "../mesh/mesh.h"

class Pyramid {
  private:
    glm::vec3 m_Coordinates;
    unsigned int m_Texture;

  public:
    void DrawingPyramid();
};