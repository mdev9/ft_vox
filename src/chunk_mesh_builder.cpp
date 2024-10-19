#include "chunk_mesh_builder.hpp"
#include <iostream>
#include "Settings.hpp"

bool is_void(const glm::ivec3& voxel_pos, const std::vector<uint8_t>& chunk_voxels) {
    int x = voxel_pos.x;
    int y = voxel_pos.y;
    int z = voxel_pos.z;

    // Out-of-bounds positions are considered void
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        //std::cout << "Out of bounds at (" << x << ", " << y << ", " << z << "), considering it void.\n";
        return true;
    }

    // Check the voxel data at the given position
    int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
    //std::cout << "Checking voxel at (" << x << ", " << y << ", " << z << ") with voxelID = " << voxelID << "\n";
    
    return voxelID == 0;
}

int addData(std::vector<uint8_t>& vertexData, int index, std::initializer_list<std::vector<int>> vertices) {
	//std::cout << "addData method called \n";
    for (const auto& vertex : vertices) {
        for (const auto& attr : vertex) {
			//std::cout << "added a vertex" << std::endl;
            vertexData[index++] = static_cast<uint8_t>(attr);
        }
    }
    return index;
}

std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size) {
	//std::cout << CHUNK_SIZE <<std::endl << CHUNK_VOL << std::endl << CHUNK_AREA << std::endl;
	//std::cout << "Format_size: " << format_size << std::endl;
    std::vector<uint8_t> vertexData(CHUNK_VOL * 18 * format_size);
    int index = 0;

	/*
	std::cout << "Chunk Voxel Data:\n";
	for (unsigned int i = 0; i < chunk_voxels.size(); ++i) {
		std::cout << static_cast<int>(chunk_voxels[i]) << " ";
	}
	std::cout << std::endl;
	*/

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelID = chunk_voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];
                if (!voxelID)
				{
					std::cout << "no voxel at " << x << ", " << z << ", " << y << std::endl;
					continue;
				}

                // Top face
                if (is_void({x, y + 1, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y + 1, z, voxelID, 0};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 0};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 0};
                    std::vector<int> v3 = {x, y + 1, z + 1, voxelID, 0};

					/*
					std::cout << "Adding Top Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                }

                // Bottom face
                if (is_void({x, y - 1, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 1};
                    std::vector<int> v1 = {x + 1, y, z, voxelID, 1};
                    std::vector<int> v2 = {x + 1, y, z + 1, voxelID, 1};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 1};

					/*
					std::cout << "Adding Bottom Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                }

                // Right face
                if (is_void({x + 1, y, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x + 1, y, z, voxelID, 2};
                    std::vector<int> v1 = {x + 1, y + 1, z, voxelID, 2};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 2};
                    std::vector<int> v3 = {x + 1, y, z + 1, voxelID, 2};

					/*
					std::cout << "Adding Right Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Left face
                if (is_void({x - 1, y, z}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 3};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 3};
                    std::vector<int> v2 = {x, y + 1, z + 1, voxelID, 3};
                    std::vector<int> v3 = {x, y, z + 1, voxelID, 3};

					/*
					std::cout << "Adding Left Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }

                // Back face
                if (is_void({x, y, z - 1}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z, voxelID, 4};
                    std::vector<int> v1 = {x, y + 1, z, voxelID, 4};
                    std::vector<int> v2 = {x + 1, y + 1, z, voxelID, 4};
                    std::vector<int> v3 = {x + 1, y, z, voxelID, 4};

					/*
					std::cout << "Adding Back Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                }

                // Front face
                if (is_void({x, y, z + 1}, chunk_voxels)) {
                    std::vector<int> v0 = {x, y, z + 1, voxelID, 5};
                    std::vector<int> v1 = {x, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v2 = {x + 1, y + 1, z + 1, voxelID, 5};
                    std::vector<int> v3 = {x + 1, y, z + 1, voxelID, 5};

					/*
					std::cout << "Adding Front Face Vertices: ";
                    for (const auto& v : {v0, v1, v2, v3}) {
                        std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << ", " << v[4] << ") ";
                    }
                    std::cout << std::endl;
					*/

                    index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                }
            }
        }
    }

    vertexData.resize(index);

	/*
	std::cout << "Vertex Data Size: " << vertexData.size() << std::endl;
    std::cout << "Vertex Data:\n";
    for (size_t i = 0; i < vertexData.size(); i += format_size) {
        std::cout << "Vertex " << (i / format_size) << ": ";
        for (int j = 0; j < format_size; ++j) {
            if (i + j < vertexData.size()) {
                std::cout << static_cast<int>(vertexData[i + j]) << " ";
            }
        }
        std::cout << "\n";
    }
	*/

    return vertexData;
}

/*
std::vector<uint8_t> build_chunk_mesh(const std::vector<uint8_t>& chunk_voxels, int format_size) {
	(void)chunk_voxels;
	(void)format_size;
    std::vector<uint8_t> vertexData;

    // Define vertices for a single triangle
    std::vector<uint8_t> v0 = { 0, 0, 0, 1, 0 }; // Position (0, 0, 0), VoxelID 1, FaceID 0
    std::vector<uint8_t> v1 = { 1, 0, 0, 1, 0 }; // Position (1, 0, 0), VoxelID 1, FaceID 0
    std::vector<uint8_t> v2 = { 0, 1, 0, 1, 0 }; // Position (0, 1, 0), VoxelID 1, FaceID 0

    // Add the triangle vertices to vertexData
    vertexData.insert(vertexData.end(), v0.begin(), v0.end());
    vertexData.insert(vertexData.end(), v1.begin(), v1.end());
    vertexData.insert(vertexData.end(), v2.begin(), v2.end());

    return vertexData;
}

*/
