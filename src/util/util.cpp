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

std::vector<Planet*> LoadPlanets(bool isAcademic) {
    std::vector<Planet*> planets;
    auto planetsConfig = YAML::LoadFile("config/planets.yml");
    int c = 0;
    for (auto it = planetsConfig.begin(); it != planetsConfig.end(); it++) {
        std::string texture = it->second["texture"].as<std::string>();
        std::vector<float> coordinates = it->second["coordinates"].as<std::vector<float>>();
        glm::vec3 glm_coordinates = glm::vec3(coordinates[0], coordinates[1], coordinates[2]);
        
        planets.push_back(new Planet(it->second, it->first.as<std::string>(), isAcademic));
    }

    return planets;
}

void addMonth(struct std::tm& tm) {
    tm.tm_mon += 1;
    if (tm.tm_mon >= 12) {
        tm.tm_mon = 1;
        tm.tm_year += 1;
    }
}

void addDay(struct std::tm& tm) {
    tm.tm_mday += 1;
    if (tm.tm_mon == 2) { //its february
        if (tm.tm_year % 4 == 0) {
            if (tm.tm_mday >= 29) {
                tm.tm_mday = 1;
                addMonth(tm);
            }
        } else {
            if (tm.tm_mday >= 28) {
                tm.tm_mday = 1;
                addMonth(tm);
            }
        }
        return;
    }

    if (tm.tm_mon == 1 || tm.tm_mon == 3 || tm.tm_mon == 5 || tm.tm_mon == 7 || tm.tm_mon == 8 || tm.tm_mon == 10 || tm.tm_mon == 12) {
        if (tm.tm_mday >= 31) {
            tm.tm_mday = 1;
            addMonth(tm);
        }
    } else {
        if (tm.tm_mday >= 30) {
            tm.tm_mday = 1;
            addMonth(tm);
        }
    }
}
void addHour(struct std::tm& tm) {
    tm.tm_hour += 1;
    if (tm.tm_hour >= 23) {
        tm.tm_hour = 0;
        addDay(tm);
    }
}

void addMin(struct std::tm& tm) {
    tm.tm_min += 1;
    if (tm.tm_min >= 59) {
        tm.tm_min = 0;
        addHour(tm);
    }
}

} // namespace util
