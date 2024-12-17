#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;
layout(location = 2) flat in uint fragTexIndex;

layout(location = 0) out vec4 outColor;

layout(binding = 2) uniform sampler2D textureArray[8];

void main() {
    outColor = texture(textureArray[fragTexIndex % 8], fragUV);
}