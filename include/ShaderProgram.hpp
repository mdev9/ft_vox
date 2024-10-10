#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <GL/glew.h>
#include <string>

class ShaderProgram {
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    void use();
    GLuint getID() const;

private:
    GLuint programID;

    std::string readShaderSource(const char* filePath);
    void checkCompileErrors(GLuint shader, const std::string& type); // Add this line
    void linkProgram();
};

#endif // SHADERPROGRAM_HPP
