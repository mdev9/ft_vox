#ifndef BASEMESH_HPP
#define BASEMESH_HPP

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class BaseMesh {
	public:
		virtual ~BaseMesh() = default;
		virtual void draw() const = 0;

	protected:
		GLuint VAO, VBO, UVBO, NBO;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		void setupMesh();
};

#endif
