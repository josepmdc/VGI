#pragma once

#include <glad/glad.h>
#include <vector>

#include "mesh.h"

class Sphere : public Mesh {
  private:
    float m_Radius = 1.0f;
    int m_SectorCount = 36;
    int m_StackCount = 18;

  public:
    ~Sphere();
    Sphere(float r, int sectors, int stacks);
    float GetRadius() { return m_Radius; }
    void Draw();
};
