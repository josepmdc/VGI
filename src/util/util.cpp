#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <yaml-cpp/yaml.h>

#include "util.h"

namespace util {

unsigned int LoadTexture(std::string path) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

std::vector<Planet*> LoadPlanets() {
    std::vector<Planet*> planets;
    auto planetsConfig = YAML::LoadFile("config/planets.yml");
    for (auto it = planetsConfig.begin(); it != planetsConfig.end(); it++) {
        std::string texture = planetsConfig[it->first]["texture"].as<std::string>();
        std::vector<float> coordinates = planetsConfig[it->first]["coordinates"].as<std::vector<float>>();
        glm::vec3 asdf = glm::vec3(coordinates[0], coordinates[1], coordinates[2]);
        Planet* planet = new Planet(0.5f, 36, 18, asdf, texture);
        planets.push_back(planet);
    }
    return planets;
}

} // namespace util
