// https://github.com/1kar/OpenGL-SolarSystem/blob/master/Sphere.h
#pragma once

#include <glad/glad.h>
#include <vector>

class Sphere {
  private:
    std::vector<float> sphere_vertices;
    std::vector<float> sphere_texcoord;
    std::vector<int> sphere_indices;
    GLuint VBO, VAO, EBO;
    float radius = 1.0f;
    int sectorCount = 36;
    int stackCount = 18;

  public:
    ~Sphere();
    Sphere(float r, int sectors, int stacks);
    void Draw();
};
