#ifndef QUADMESH_HPP
#define QUADMESH_HPP

#include "BaseMesh.hpp"

class QuadMesh : public BaseMesh {
	public:
		QuadMesh();
		void draw() const override;

	private:
		void initializeMesh();
};

#endif
