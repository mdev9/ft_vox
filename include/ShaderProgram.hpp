#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <GL/glew.h>
#include <iostream>

class ShaderProgram {
public:
    unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);

private:
    void checkShaderCompileErrors(unsigned int shader, const std::string& type);
};

#endif
