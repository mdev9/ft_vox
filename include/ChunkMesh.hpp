#ifndef CHUNKMESH_HPP
#define CHUNKMESH_HPP

#include "BaseMesh.hpp"
#include "Chunk.hpp"

class Chunk;

class ChunkMesh : public BaseMesh {
public:
    ChunkMesh(Chunk* chunk);
    ~ChunkMesh();

    std::vector<uint32_t> getVertexData() override;
	void render() override;
	void rebuild();

private:
    Chunk* chunk;
	int formatSize;
    GLuint vao, vbo;
    int vertexCount;
    std::vector<std::string> vboFormat;
    std::vector<std::string> attrs;

    int calculateFormatSize(const std::vector<std::string>& format);
    GLuint createVAO();
};

#endif
