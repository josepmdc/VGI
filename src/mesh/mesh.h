#pragma once

#include <vector>
#include <glad/glad.h>

class Mesh {
  protected:
    std::vector<float> m_Vertices;
    std::vector<int> m_Indices;
    GLuint m_VBO, m_VAO, m_EBO;

  public:
    virtual void Draw() = 0;
};
