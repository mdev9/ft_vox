#include "ShaderProgram.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Load the vertex shader
    std::string vertexCode = readShaderSource(vertexPath);
    const char* vertexSource = vertexCode.c_str();

    // Load the fragment shader
    std::string fragmentCode = readShaderSource(fragmentPath);
    const char* fragmentSource = fragmentCode.c_str();

    // Compile shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX"); // Check for compile errors

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT"); // Check for compile errors

    // Link the program
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    linkProgram();

    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

std::string ShaderProgram::readShaderSource(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;

    if (!file.is_open()) {
        std::cerr << "Could not open shader file: " << filePath << std::endl;
        return ""; // Return empty string if the file can't be opened
    }

    buffer << file.rdbuf();
    return buffer.str();
}

void ShaderProgram::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }
}

void ShaderProgram::linkProgram() {
    glLinkProgram(programID);
}

void ShaderProgram::use() {
    glUseProgram(programID);
}

GLuint ShaderProgram::getID() const {
    return programID;
}
