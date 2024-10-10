#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Texture.hpp"

Texture::Texture(const std::string& path) {
    loadTexture(path);
}

void Texture::loadTexture(const std::string& path) {
	// Load texture (assuming you're using SOIL or similar)
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Load image and generate texture
	int width, height, channels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Set texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	} else {
		std::cout << "Failed to load texture: " << path << std::endl;
	}
	stbi_image_free(data);
}

GLuint Texture::getID() const {
    return textureID; // Return the texture ID
}

Texture::~Texture() {
    glDeleteTextures(1, &textureID); // Cleanup the texture
}
