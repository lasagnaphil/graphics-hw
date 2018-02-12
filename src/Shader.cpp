//
// Created by lasagnaphil on 2017-03-27.
//

#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "GLUtils.h"

GLuint compileShader(GLenum type, const GLchar *source) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_TRUE) {
        std::cout << "Shader (type " << type << ") is compiled successfully!" << std::endl;
    } else {
        std::cout << "Shader (type " << type << ") compile failed!" << std::endl;
    }

    std::cout << "Compile log: " << std::endl;
    char compileInfo[512];
    glGetShaderInfoLog(shader, 512, NULL, compileInfo);
    std::cout << compileInfo << std::endl;

    return shader;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    program = glCreateProgram();

    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexFile;
    std::ifstream fragmentFile;

    vertexFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);

        std::stringstream vertexBuffer, fragmentBuffer;
        vertexBuffer << vertexFile.rdbuf();
        fragmentBuffer << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexBuffer.str();
        fragmentCode = fragmentBuffer.str();
    } catch (std::ifstream::failure e) {
        std::cout << "Error: shader file not successfully read" << std::endl;
    }

    const GLchar* vertexCodePtr = vertexCode.data();
    const GLchar* fragmentCodePtr = fragmentCode.data();

    GLuint vertexShaderPtr = compileShader(GL_VERTEX_SHADER, vertexCodePtr);
    GLuint fragmentShaderPtr = compileShader(GL_FRAGMENT_SHADER, fragmentCodePtr);
    glAttachShader(program, vertexShaderPtr);
    glAttachShader(program, fragmentShaderPtr);
    glLinkProgram(program);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Error: shader program linking failed" << infoLog << std::endl;
    }

    //glDeleteShader(vertexShaderPtr);
    //glDeleteShader(fragmentShaderPtr);
}

void Shader::setProgram(GLuint program) {
    this->program = program;
}

void Shader::use() {
    glUseProgram(this->program);
}

void Shader::setBool(const char* name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name), (int)value);
}

void Shader::setBool(GLint uniID, bool value) const {
    glUniform1i(uniID, (int)value);
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setInt(GLint uniID, int value) const {
    glUniform1i(uniID, value);
}

void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(GLint uniID, float value) const {
    glUniform1f(uniID, value);
}

void Shader::setMat4(const char* name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(GLint uniID, const glm::mat4& value) const {
    glUniformMatrix4fv(uniID, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void Shader::setVec3(GLint uniID, const glm::vec3& value) const {
    glUniform3fv(uniID, 1, glm::value_ptr(value));
}

GLint Shader::getUniformLocation(const char* name) {
    return glGetUniformLocation(program, name);
}

