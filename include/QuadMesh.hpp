#include "BaseMesh.hpp"
#include "ShaderProgram.hpp"
#include <glm/glm.hpp>

class QuadMesh : public BaseMesh {
	public:
		QuadMesh(ShaderProgram* shaderProgram);
		std::vector<float> getVertexData() override;

	private:
		void createMesh();
};
