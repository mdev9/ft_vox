#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in int voxel_id;
layout (location = 2) in int face_id;
layout (location = 3) in int ao_id;

uniform mat4 m_proj;
uniform mat4 m_view;
uniform mat4 m_model;

out vec3 voxel_color;
out vec2 uv;
out float shading;

const float ao_values[4] = float[4](0.1f, 0.25f, 0.5f, 1.0f);

const float face_shading[6] = float[6](
	1.0, 0.5, //top bottom
	0.5, 0.8, //right left
	0.5, 0.8  //front back
);

const vec2 uv_coords[4] = vec2[4](
	vec2(0, 0), vec2(0, 1),
	vec2(1, 0), vec2(1, 1)
);

const int uv_indices[12] = int[12](
	1, 0, 2, 1, 2, 3,	//even face
	3, 0, 2, 3, 1, 0	//odd face
);

vec3 hash31(float p) {
    vec3 p3 = fract(vec3(p * 21.2) * vec3(0.1031, 0.1030, 0.0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xxy + p3.yzz) * p3.zyx) + 0.05;
}

void main() {
	int uv_index = gl_VertexID % 6 + (face_id & 1) * 6;
    uv = vec2(1.0 - uv_coords[uv_indices[uv_index]].x, uv_coords[uv_indices[uv_index]].y); // Flip horizontaly
	voxel_color = hash31(voxel_id);
	shading = face_shading[face_id] * ao_values[ao_id];
	gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);
}
