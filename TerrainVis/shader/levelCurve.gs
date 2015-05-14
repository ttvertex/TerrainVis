#version 400

//IO types
layout (triangles) in;
//layout(triangle_strip, max_vertices = 3) out;
layout(line_strip, max_vertices = 6) out;

//from VS
in vec3 VNormal[];
in vec3 VPosition[];
in vec3 WorldPosition[];

//out vec3 GNormal;
//out vec3 GPosition;
flat out int  GCurveLevel;

uniform mat4 ModelViewMatrix;

void main(){
	// worldPosition can be calculated as ((inverse(ModelViewMatrix) * vec4(VPosition[0], 1.0))
	GCurveLevel = (WorldPosition[0].y < 0.1f) ?  0 : 1; 

    //GNormal = VNormal[0];
    //GPosition = VPosition[0];
    //gl_Position = gl_in[0].gl_Position;
    //EmitVertex();

	//GNormal = VNormal[1];
    //GPosition = VPosition[1];
    //gl_Position = gl_in[1].gl_Position;
    //EmitVertex();

	//GNormal = VNormal[2];
    //GPosition = VPosition[2];
    //gl_Position = gl_in[2].gl_Position;
    //EmitVertex();



	//p0-p1
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	//p0-p2
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();
	
	//p1-p2
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();
}