#version 400

//IO types
layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//from VS
in vec3 VNormal[];
in vec3 VPosition[];

out vec3 GNormal;
out vec3 GPosition;

void main(){
    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

	GNormal = VNormal[1];
    GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

	GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

	EndPrimitive();
}