#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

const std::string VS_FILE_EXT = ".vert";
const std::string FS_FILE_EXT = ".frag";

class Shader {
    private:
        
        GLuint ID;
        std::unordered_map<std::string, int> m_UniformCache;

    public:
        Shader(const std::string filepath);
        ~Shader();
        
        std::string ParseShader(const std::string& filepath, const std::string type);
        
        void Bind();
        void Unbind();

        int GetUniformLocation(const std::string& name);
        GLuint returnId(){
            return ID;
        }

    private:
        unsigned int Create(std::string vertexSrc, std::string fragmentSrc);
        unsigned int Compile(unsigned int type, const std::string &source);
};