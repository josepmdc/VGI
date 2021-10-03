#include "shader.h"

Shader::Shader(const std::string filepath) {
    std::string vertexSrc = ParseShader(filepath, VS_FILE_EXT);
    std::string fragmentSrc = ParseShader(filepath, FS_FILE_EXT);
    ID = Create(vertexSrc, fragmentSrc);
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

std::string Shader::ParseShader(const std::string& filepath, const std::string type) {
    std::ifstream stream(filepath + type);
    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
}

unsigned int Shader::Create(std::string vertexSrc, std::string fragmentSrc) {
    unsigned int program = glCreateProgram();
    unsigned int vs = Compile(GL_VERTEX_SHADER, vertexSrc);
    unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentSrc);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &lenght);

        char* infoLog = new char[lenght];
        glGetShaderInfoLog(program, lenght, NULL, infoLog);

        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;

        delete[] infoLog;
    }

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::Compile(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);

    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int lenght;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &lenght);

        char* infoLog = new char[lenght];
        glGetShaderInfoLog(shader, lenght, NULL, infoLog);

        std::cout
            << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n"
            << infoLog
            << std::endl;

        delete[] infoLog;
        glDeleteShader(shader);

        return 0;
    }
    
    return shader;
}

void Shader::Bind() {
    glUseProgram(ID);
}

void Shader::Unbind() {
    glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformCache.find(name) != m_UniformCache.end()) {
        return m_UniformCache[name];
    }
    
    int location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::cout << "ERROR: Uniform \"" << name << "\" not found";
        return -1;
    }

    m_UniformCache[name] = location;
    return location;    
}