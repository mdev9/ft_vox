#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>
#include <iostream>

class Texture {
public:
    Texture(const std::string& path);
    GLuint getID() const;
    ~Texture();

private:
    GLuint textureID;
    void loadTexture(const std::string& path);
};

#endif // TEXTURE_HPP
