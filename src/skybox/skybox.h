#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>

#include "../shader/shader.h"

class Skybox {
  private:
    GLuint m_CubemapTexture;
    Shader m_Shader;
    GLuint m_VAO;
    GLuint m_VBO;

  public:
    Skybox(std::string shader, std::vector<std::string> faces);
    void CubeSkybox(GLdouble dSize);
    void LoadSkyboxCubeVAO();
    void LoadCubemap(std::vector<std::string> faces);
    void OnVistaSkyBox();
    void DrawCubeSkybox();
    void DeleteVAO(GLint k);
    void Draw(glm::mat4 projection, glm::mat4 view);
};
