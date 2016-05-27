#version 400

//IO types
layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out; // a triangle

//from VS
in vec3 VNormal[];

out vec3 GWorldPosition;
out vec3 GNormal;
out vec3 GPosition;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

void main(){

	////p0
	GNormal = VNormal[0];
    GWorldPosition = vec3(gl_in[0].gl_Position);
    gl_Position = MVP * gl_in[0].gl_Position;
    EmitVertex();

	////p1
	GNormal = VNormal[1];
    GWorldPosition = vec3(gl_in[1].gl_Position);
    gl_Position = MVP * gl_in[1].gl_Position;
    EmitVertex();

	////p2
	GNormal = VNormal[2];
    GWorldPosition = vec3(gl_in[2].gl_Position);
    gl_Position = MVP * gl_in[2].gl_Position;
    EmitVertex();
	EndPrimitive();
}

