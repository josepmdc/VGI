
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


#include <stb_image.h>
#include <vector>

GLint VBOId[40] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

GLint VAOId[40] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
GLuint skC_VAOID;
GLuint cubemapTexture;	

unsigned char*
SOIL_load_image(
    const char* filename,
    int* width, int* height, int* channels,
    int force_channels);


void SOIL_free_image_data(
    unsigned char* img_data);

enum {
    SOIL_LOAD_AUTO = 0,
    SOIL_LOAD_L = 1,
    SOIL_LOAD_LA = 2,
    SOIL_LOAD_RGB = 3,
    SOIL_LOAD_RGBA = 4
};

GLuint loadCubeSkybox_VAO() {
    GLfloat skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    // skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    VAOId[26] = skyboxVAO;
    return skyboxVAO;
}

unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height; // nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        //unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            //stbi_image_free(data);
            SOIL_free_image_data(data);
        } else { //std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            fprintf(stderr, "Cubemap texture failed to load at path: %s \n", faces[i].c_str());
            //stbi_image_free(data);
            SOIL_free_image_data(data);
            return 0;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void OnVistaSkyBox() {
   



      
    // cargar skybox shaders
    // 
    

    // Càrrega VAO Skybox Cube
   
     skC_VAOID = loadCubeSkybox_VAO();

    if (!cubemapTexture) { // load Skybox textures
        // -------------
        std::vector<std::string> faces = { ".\\textures\\skybox\\right.jpg",
                                           ".\\textures\\skybox\\left.jpg",
                                           ".\\textures\\skybox\\top.jpg",
                                           ".\\textures\\skybox\\bottom.jpg",
                                           ".\\textures\\skybox\\front.jpg",
                                           ".\\textures\\skybox\\back.jpg" };
        cubemapTexture = loadCubemap(faces);
    }

    // Entorn VGI: Activació el contexte OpenGL. Permet la coexistencia d'altres contextes de generació


    // Crida a OnPaint() per redibuixar l'escena

}

void drawCubeSkybox() {
    GLuint vaoId = 0;

    // Recuperar identificadors VAO, VBO dels vectors VAOId, VBOId
    vaoId = VAOId[26];
    if (vaoId != -1) {
        glBindVertexArray(vaoId);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}

void deleteVAO(GLint k) {
    GLuint vaoId;
    GLuint vboId;

    vaoId = VAOId[k];
    vboId = VBOId[k];

    if (vaoId != -1) {
        glBindVertexArray(vaoId);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        glDisableVertexAttribArray(3);

        // It is good idea to release VBOs with ID 0 after use.
        // Once bound with 0, all pointers in gl*Pointer() behave as real
        // pointer, so, normal vertex array operations are re-activated
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(40, &vboId);

        // Unbind and delete VAO
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vaoId);

        VBOId[k] = -1;
        VAOId[k] = -1;
    }
}
void CubeSkybox(GLdouble dSize) {
    loadCubeSkybox_VAO();
    drawCubeSkybox();
    deleteVAO(26);
}