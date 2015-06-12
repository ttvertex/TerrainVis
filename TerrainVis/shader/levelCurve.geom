#version 400

//IO types
layout (triangles) in;
layout(triangle_strip, max_vertices = 9) out;
//layout(line_strip, max_vertices = 6) out;
//layout(points, max_vertices = 6) out;

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

    /////////// color interpolation along the edge

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
		//GColor = mix(colorA, colorB, float(val));
		GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
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
		//GColor = mix(colorA, colorB,  float(val));
		GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
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
		//GColor = mix(colorA, colorB, float(val));
		GColor = vec4( 1.0f,0.1f,0.0f, 1.0f );
	}else{
		GColor = (WorldPosition[2].y < h) ?  colorA : colorB;
	}
	GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();
 //   ///////////////////////////

 //////////////// DRAWING WITH LINES ---> NO FILL FACE
 	//p0
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
    
	////p1
	//GNormal = VNormal[1];
 //   GPosition = VPosition[1];
	//GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[1].gl_Position;
 //   EmitVertex();
	////p2
	//GNormal = VNormal[2];
 //   GPosition = VPosition[2];
	//GColor = (WorldPosition[2].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[2].gl_Position;
 //   EmitVertex();

	////p2
	//GNormal = VNormal[2];
 //   GPosition = VPosition[2];
	//GColor = (WorldPosition[2].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[2].gl_Position;
 //   EmitVertex();
	////p0
	//GNormal = VNormal[0];
 //   GPosition = VPosition[0];
	//GColor = (WorldPosition[0].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
 //   gl_Position = gl_in[0].gl_Position;
 //   EmitVertex();
    ///////////////////////////

	///////////// levelCurve triangle
	//vec4 v1 = vec4(WorldPosition[0], 1.0f);
 //   vec4 v2 = vec4(WorldPosition[1], 1.0f);
 //   vec4 v3 = vec4(WorldPosition[2], 1.0f);
    
	//bool v1v2b, v1v3b, v2v3b;
 //   vec4 v1v2, v1v3, v2v3;
	//v1v2b = v1v3b = v2v3b = false;

 //   if ((v1.y <= h && v2.y >= h) || (v1.y >= h && v2.y <= h)){ // intersect v1->v2
	//	v1v2 = findPlaneLineIntersectionPoint(v1, v2, h);
	//	//v1v2 = vec4((v1.x+v2.x)/2,(v1.y+v2.y)/2,(v1.z+v2.z)/2,1.0);
 //       v1v2b = true;
 //   }

	//if ((v1.y <= h && v3.y >= h) || (v1.y >= h && v3.y <= h)){ // intersect v1->v3
	//	v1v3 = findPlaneLineIntersectionPoint(v1, v3, h);
	//	//v1v3 = vec4((v1.x+v3.x)/2,(v1.y+v3.y)/2,(v1.z+v3.z)/2,1.0);
 //       v1v3b = true;
 //   }

	//if ((v2.y <= h && v3.y >= h) || (v2.y >= h && v3.y <= h)){ // intersect v2->v3
	//	v2v3 = findPlaneLineIntersectionPoint(v2, v3, h);
	//	//v2v3 = vec4((v2.x+v3.x)/2,(v2.y+v3.y)/2,(v2.z+v3.z)/2,1.0);
 //       v2v3b = true;
 //   }


	//if(v1v2b && v1v3b){
		
	//	//p0 -> v1v2
	//	GColor = (v1v2.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v1v2 = MVP * v1v2;
	//	GNormal = VNormal[0];
 //       GPosition = vec3(v1v2);
 //       gl_Position = v1v2;
 //       EmitVertex();
 //       //p1 = v2
	//	GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	GNormal = VNormal[1];
 //       GPosition = VPosition[1];
 //       gl_Position = gl_in[1].gl_Position;
 //       EmitVertex();
 //       //p2 -> v1v3
	//	GColor = (v1v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v1v3 = MVP * v1v3;
	//	GNormal = VNormal[2];
 //       GPosition = vec3(v1v3);
 //       gl_Position = v1v3;
 //       EmitVertex();
 //   }

	//else if(v1v2b && v2v3b){
 //       //p0 -> v1v2
	//	GColor = (v1v2.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v1v2 = MVP * v1v2;
	//	GNormal = VNormal[0];
 //       GPosition = vec3(v1v2);
 //       gl_Position = v1v2;
 //       EmitVertex();
 //       //p1 = v2
	//	GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	GNormal = VNormal[1];
 //       GPosition = VPosition[1];
 //       gl_Position = gl_in[1].gl_Position;
 //       EmitVertex();
 //       //p2 -> v2v3
	//	GColor = (v2v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v2v3 = MVP * v2v3;
	//	GNormal = VNormal[2];
 //       GPosition = vec3(v2v3);
 //       gl_Position = v2v3;
 //       EmitVertex();
 //   }

	//	else if(v2v3b && v1v3b){
 //       //p0 -> v2v3
	//	GColor = (v2v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v2v3 = MVP * v2v3;
	//	GNormal = VNormal[0];
 //       GPosition = vec3(v2v3);
 //       gl_Position = v2v3;
 //       EmitVertex();
 //       //p1 = v3
	//	GColor = (WorldPosition[2].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	GNormal = VNormal[2];
 //       GPosition = VPosition[2];
 //       gl_Position = gl_in[2].gl_Position;
 //       EmitVertex();
 //       //p2 -> v1v3
	//	GColor = (v1v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
	//	v1v3 = MVP * v1v3;
	//	GNormal = VNormal[2];
 //       GPosition = vec3(v1v3);
 //       gl_Position = v1v3;
 //       EmitVertex();
 //   }


	EndPrimitive();
}

