#type vertex
#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out vec4 v_Color;
out flat float v_TexIndex;
out float v_TilingFactor;
void main()
{	
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
}

#type fragment
#version 450

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 color1;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	color1 = vec4(1.0, 0.0, 1.0, 1.0);
}