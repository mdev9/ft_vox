#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class BaseMesh {
public:
    BaseMesh();
    virtual ~BaseMesh();

    virtual std::vector<glm::vec3> getVertexData() = 0;
    GLuint getVAO();
	GLuint getProgram();

protected:
    GLuint vao;
    GLuint vbo;
    GLuint program;
    int vertexCount;
    void setupMesh();
};
