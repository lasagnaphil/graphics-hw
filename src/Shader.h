//
// Created by lasagnaphil on 2017-03-27.
//

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

class Shader {
public:

    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    void use();

    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;

    GLuint program;
};

