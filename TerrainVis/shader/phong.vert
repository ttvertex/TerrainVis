#version 430
layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexColor;
layout (location=2) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat4 NormalViewMatrix;
uniform mat4 ProjectionMatrix;
uniform vec3 LightPos;

out vec4 Color;
out vec3 N;
out vec3 v;

void main(void)  
{
   Color = vec4(VertexColor, 1.0f);  
   v = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0f));       
   N = normalize(vec3(ModelViewMatrix * vec4(VertexNormal, 1.0f)));
   gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(VertexPosition, 1.0f);  
}
     