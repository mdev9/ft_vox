#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "QuadMesh.hpp"
#include "VoxelEngine.hpp"

class VoxelEngine;

class Scene {
	public:
		Scene(VoxelEngine* engine);
		~Scene();

		void addMesh(BaseMesh* mesh);
		void update();
		void render();

	private:
		std::vector<BaseMesh*> meshes;
};

#endif
