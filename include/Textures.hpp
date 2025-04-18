#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <GL/glew.h>
#include <Settings.hpp>
#include <iostream>

class Textures {
	public:
		Textures();
		~Textures();

		GLuint loadTexture(const std::string& path);
	
	private:
		GLuint textureID;
};

#endif
