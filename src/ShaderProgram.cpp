#include "ShaderProgram.hpp"
#include "VoxelEngine.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(Player* player)
{
	this->player = player;
	
    // Load and compile shaders, then link into a program
    chunkProgram = loadProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
    // Set initial uniforms
    setUniformsOnInit();
}

void ShaderProgram::setUniformsOnInit() {
	m_proj = player->m_proj;
    glUseProgram(chunkProgram);

    // Set the projection matrix (m_proj)
    glUniformMatrix4fv(glGetUniformLocation(chunkProgram, "m_proj"), 1, GL_FALSE, glm::value_ptr(m_proj));

    // Cache the model matrix location
    m_modelLoc = glGetUniformLocation(chunkProgram, "m_model");
    // Set a default model matrix (identity matrix)
    glUniformMatrix4fv(m_modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	// Set texture uniform
    glUniform1i(glGetUniformLocation(chunkProgram, "u_texture_0"), 0);
}

void ShaderProgram::update() {
	m_view = player->m_view;
    glUseProgram(chunkProgram);

    // Update the view matrix (m_view)
    glUniformMatrix4fv(glGetUniformLocation(chunkProgram, "m_view"), 1, GL_FALSE, glm::value_ptr(m_view));
}

GLuint ShaderProgram::loadProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    // Load vertex and fragment shaders
    std::string vertexCode = loadShaderCode(vertexPath);
    std::string fragmentCode = loadShaderCode(fragmentPath);

    GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // Create and link the program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		glDeleteProgram(shaderProgram);
		return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

	if (shaderProgram == 0) {
		std::cerr << "Shader program is not created!" << std::endl;
	}

    return shaderProgram;
}

std::string ShaderProgram::loadShaderCode(const std::string& filePath) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;

    shaderFile.open(filePath);
    if (shaderFile.fail()) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    shaderStream << shaderFile.rdbuf();

    shaderFile.close();

    return shaderStream.str();
}

GLuint ShaderProgram::compileShader(const char* shaderSource, GLenum shaderType) {
	if (shaderSource == nullptr || strlen(shaderSource) == 0) {
        std::cerr << "ERROR::SHADER::SOURCE_IS_EMPTY\n";
        return 0;
    }

    GLuint shader = glCreateShader(shaderType);

    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(shader);
		return 0;
    }

    return shader;
}

GLuint ShaderProgram::getProgram() {
	return chunkProgram;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(chunkProgram);
}
