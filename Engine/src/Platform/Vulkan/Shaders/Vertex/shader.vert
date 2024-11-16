#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;


struct ObjectData{
	vec3 color;
	mat4 model;
};

layout(std140, binding = 1) readonly buffer ObjectBuffer{
	ObjectData objects[];
} objectBuffer;

layout(location = 0)in vec3 inPosition;
layout(location = 1)in vec3 inNormal;
layout(location = 2)in vec2 inUV;

layout(location = 0) out vec3 fragColor;

void main() {
    gl_Position = ubo.proj * ubo.view * objectBuffer.objects[gl_InstanceIndex].model/* ubo.model*/ * vec4(inPosition, 1.0);
    fragColor = objectBuffer.objects[gl_InstanceIndex].color;
}