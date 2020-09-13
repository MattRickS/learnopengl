#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 normal;
out vec3 fragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	fragPos = vec3(view * model * vec4(aPos, 1.0));
	// Slow to inverse on the GPU per vertex.
	normal = normalMatrix * aNormal;
}