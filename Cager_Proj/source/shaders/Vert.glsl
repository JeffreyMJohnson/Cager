#version 330

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec4 Normal;
layout(location=3) in vec4 Tangent;
layout(location=4) in vec2 TexCoord;

out vec4 vColor;

uniform mat4 ProjectionView;

void main() 
{
	vColor = Color;
	gl_Position= ProjectionView * Position;
}