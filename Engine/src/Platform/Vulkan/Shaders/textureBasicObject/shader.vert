#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;


struct ObjectData{
	uint textureIndex;
	mat4 model;
};

layout(std430, set = 1, binding = 1) readonly buffer ObjectBuffer{
	ObjectData objects[];
} objectBuffer;

layout(location = 0)in vec3 inPosition;
layout(location = 1)in vec3 inNormal;
layout(location = 2)in vec2 inUV;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out uint fragTexIndex;

void main() {
    gl_Position = ubo.proj * ubo.view * objectBuffer.objects[gl_InstanceIndex].model * vec4(inPosition, 1.0);
    fragTexIndex = objectBuffer.objects[gl_InstanceIndex].textureIndex;
    fragUV=inUV;
}