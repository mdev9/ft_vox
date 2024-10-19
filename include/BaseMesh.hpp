#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class BaseMesh {
public:
    BaseMesh();
    virtual ~BaseMesh();

    virtual std::vector<float> getVertexData() = 0;
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
