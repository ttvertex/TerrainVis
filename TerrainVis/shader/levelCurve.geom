#version 400

//IO types
layout (triangles) in;
layout(triangle_strip, max_vertices = 3) out; // a triangle

struct LevelCurve {
	float Height; // height
};
uniform LevelCurve LCurve;

//from VS
in vec3 VNormal[];
in vec3 VPosition[];
in vec3 WorldPosition[];
out vec3 GNormal;
out vec3 GPosition;
out vec4 GColor;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

vec4 findPlaneLineIntersectionPoint(vec4 p0, vec4 p1, float height){
    vec4 res;
    float t;
    //line equation:  p0 + t * (p1 - p0)
	//plane equation: y = height
	//(x,y,z) = p0 + t * (p1 - p0)
	t = (height - p0.y) / (p1.y - p0.y);
	res = p0 + t * (p1 - p0);
	return res;
}

bool intersectPlaneLine(vec4 p0, vec4 p1, float h){
	const float t = 0.001; // treshold
	if ((p0.y <= h + t && p1.y >= h - t) || (p0.y >= h - t && p1.y <= h + t)){ // intersect p0->p1
		return true;
    }
	return false;
}


void main(){
    // worldPosition can be calculated as ((inverse(ModelViewMatrix) * vec4(VPosition[0], 1.0))
	float h = LCurve.Height; //0.1f;
	vec4 colorA = vec4( 0.0f,0.1f,1.0f, 1.0f );
	vec4 colorB = vec4(0.0,1.0,0.0,1.0);

 //   /////////// The regular triangle
	//////p0
	//GNormal = VNormal[0];
 //   GPosition = VPosition[0];
	//GColor = (WorldPosition[0].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[0].gl_Position;
 //   EmitVertex();
 //   //p1
	//GNormal = VNormal[1];
 //   GPosition = VPosition[1];
	//GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[1].gl_Position;
 //   EmitVertex();
 //   //p2
	//GNormal = VNormal[2];
 //   GPosition = VPosition[2];
	//GColor = (WorldPosition[2].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[2].gl_Position;
 //   EmitVertex();
 ////   ///////////////////////////

    /////////// color interpolation along the edge. the vertex receives the color according to its distance to the level curve
	vec4 v1 = vec4(WorldPosition[0], 1.0f);
    vec4 v2 = vec4(WorldPosition[1], 1.0f);
    vec4 v3 = vec4(WorldPosition[2], 1.0f);

	double v1v2d = length(v1 - v2);
	double v1v3d = length(v1 - v3);
	double v2v3d = length(v2 - v3);

	//mixval is the distance betweeen the vertex and the intersection point
	////p0
	if(intersectPlaneLine(v1, v2, h)){
		vec4 p = findPlaneLineIntersectionPoint(v1, v2, h);
		double val = (length(p - v2) / v1v2d);
		GColor = mix(colorA, colorB, clamp(float(val), 0.0f, 1.0f));
		//GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
	}else{
		GColor = (WorldPosition[0].y < h) ?  colorA : colorB;
	}
	GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    //p1
	if(intersectPlaneLine(v1,v3,h)){
		vec4 p = findPlaneLineIntersectionPoint(v1,v3,h);
		double val = (length(p - v3) / v1v3d);
		GColor = mix(colorA, colorB,  clamp(float(val), 0.0f, 1.0f));
		//GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
	}else{
		GColor = (WorldPosition[1].y < h) ?  colorA : colorB;
	}
	GNormal = VNormal[1];
    GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    //p2
	if(intersectPlaneLine(v2,v3,h)){
		vec4 p = findPlaneLineIntersectionPoint(v2,v3,h);
		double val = (length(p - v3) / v2v3d);
		GColor = mix(colorA, colorB, clamp(float(val), 0.0f, 1.0f));
		//GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
	}else{
		GColor = (WorldPosition[2].y < h) ?  colorA : colorB;
	}
	GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

	EndPrimitive();
}

