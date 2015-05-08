#version 400

//IO types
layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//from VS
in vec3 VNormal[];
in vec3 VPosition[];

out vec3 GNormal;
out vec3 GPosition;
flat out int  GCurveLevel;

uniform mat4 ModelViewMatrix;

void main(){
	GCurveLevel = ((inverse(ModelViewMatrix) * vec4(VPosition[0], 1.0)).y < 0.1f) ?  0 : 1;

    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
	//GCurveLevel = (gl_Position.y < 0.1f) ?  0 : 1;
    EmitVertex();

	GNormal = VNormal[1];
    GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
	//GCurveLevel = (gl_Position.y < 0.1f) ?  0 : 1;
    EmitVertex();

	GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
	//GCurveLevel = (gl_Position.y < 0.1f) ?  0 : 1;
    EmitVertex();

	EndPrimitive();
}