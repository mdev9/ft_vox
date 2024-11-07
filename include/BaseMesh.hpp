#include "VoxelHandler.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class BaseMesh {
	public:
		BaseMesh();
		virtual ~BaseMesh();

		virtual std::vector<uint32_t> getVertexData(VoxelHandler *handler) = 0;
		virtual void render() = 0;

		GLuint getVAO();
		GLuint getProgram();

	protected:
		GLuint vao;
		GLuint vbo;
		GLuint program;
		int vertexCount;
		void setupMesh();
};
