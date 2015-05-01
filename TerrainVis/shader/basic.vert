#version 430

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;

out vec3 Color;

uniform mat4 ModelViewMatrix;

void main()
{
    Color = VertexColor;
	//Color = vec3(1.0f,0.5f,0.0f);
	
	gl_Position = ModelViewMatrix * vec4(VertexPosition,1.0);
}
