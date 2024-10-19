#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in int voxel_id;
layout (location = 2) in int face_id;

uniform mat4 m_proj;
uniform mat4 m_view;
uniform mat4 m_model;

out vec3 voxel_color;

void main() {
	if (voxel_id % 3 == 0) {
		voxel_color = vec3(1.0, 0.0, 0.0);
	} else if (voxel_id % 3 == 1) {
		voxel_color = vec3(0.0, 1.0, 0.0);
	} else {
		voxel_color = vec3(0.0, 0.0, 1.0);
	}

	gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);
}
