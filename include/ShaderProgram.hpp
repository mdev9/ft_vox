#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ShaderProgram {
public:
    ShaderProgram(const glm::mat4& proj, const glm::mat4& view);
	~ShaderProgram();

    void setUniformsOnInit();
    void update();

private:
    GLuint quadProgram;
    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 m_model;

    GLuint getProgram(const std::string& vertexPath, const std::string& fragmentPath);
    std::string loadShaderCode(const std::string& filePath);
    GLuint compileShader(const char* shaderSource, GLenum shaderType);
};

#endif
