#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Player.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <string>

class VoxelEngine;

class ShaderProgram {
public:
    ShaderProgram(Player* player);
	~ShaderProgram();

	GLuint getProgram();

    void setUniformsOnInit();
    void update();


private:
	VoxelEngine* engine;
	Player *player;
    GLuint chunkProgram;
    glm::mat4 m_proj;
    glm::mat4 m_view;

    GLuint loadProgram(const std::string& vertexPath, const std::string& fragmentPath);
    std::string loadShaderCode(const std::string& filePath);
    GLuint compileShader(const char* shaderSource, GLenum shaderType);
};

#endif
