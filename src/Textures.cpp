#include "Textures.hpp"
#include <stb_image.h>

Textures::Textures() {
	textureID = loadTexture("assets/frame.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

GLuint Textures::loadTexture(const std::string& file_name) {
	int width, height, nrChannels;

	//stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, 0);
	if (!data) {
		std::cerr << "Failed to load texture: " << file_name << std::endl;
		return 0;
	}

	// Generate OpenGL texture ID
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load texture data into OpenGL
	if (nrChannels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else if (nrChannels == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	return textureID;
}

Textures::~Textures() {
	glDeleteTextures(1, &textureID);
}
