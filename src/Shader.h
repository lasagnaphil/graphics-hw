//
// Created by lasagnaphil on 2017-03-27.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/detail/type_mat.hpp>

class Shader {
public:

    Shader() {}
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void setProgram(GLuint program);

    void use();

    void setBool(const char* name, bool value) const;
    void setBool(GLint uniID, bool value) const;
    void setInt(const char* name, int value) const;
    void setInt(GLint uniID, int value) const;
    void setFloat(const char* name, float value) const;
    void setFloat(GLint uniID, float value) const;
    void setMat4(const char* name, const glm::mat4& value) const;
    void setMat4(GLint uniID, const glm::mat4& value) const;


    GLint getUniformLocation(const char* name);

    GLuint program;
};

