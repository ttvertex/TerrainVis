#version 400

//IO types
layout (triangles) in;
//layout(triangle_strip, max_vertices = 9) out;
layout(line_strip, max_vertices = 6) out;

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



void main(){
    // worldPosition can be calculated as ((inverse(ModelViewMatrix) * vec4(VPosition[0], 1.0))
	const float h = 0.11f;
    /////////// The regular triangle
	////p0
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
	vec4 v1 = vec4(WorldPosition[0], 1.0f);
    vec4 v2 = vec4(WorldPosition[1], 1.0f);
    vec4 v3 = vec4(WorldPosition[2], 1.0f);
    
	bool v1v2b, v1v3b, v2v3b;
    vec4 v1v2, v1v3, v2v3;
	v1v2b = v1v3b = v2v3b = false;

    if ((v1.y <= h && v2.y >= h) || (v1.y >= h && v2.y <= h)){ // intersect v1->v2
		//v1v2 = findPlaneLineIntersectionPoint(v1, v2, h);
		v1v2 = vec4((v1.x+v2.x)/2,(v1.y+v2.y)/2,(v1.z+v2.z)/2,1.0);
        v1v2b = true;
    }

	if ((v1.y <= h && v3.y >= h) || (v1.y >= h && v3.y <= h)){ // intersect v1->v3
		//v1v3 = findPlaneLineIntersectionPoint(v1, v3, h);
		v1v3 = vec4((v1.x+v3.x)/2,(v1.y+v3.y)/2,(v1.z+v3.z)/2,1.0);
        v1v3b = true;
    }

	if ((v2.y <= h && v3.y >= h) || (v2.y >= h && v3.y <= h)){ // intersect v2->v3
		//v2v3 = findPlaneLineIntersectionPoint(v2, v3, h);
		v2v3 = vec4((v2.x+v3.x)/2,(v2.y+v3.y)/2,(v2.z+v3.z)/2,1.0);
        v2v3b = true;
    }


	if(v1v2b && v1v3b){
		
		//p0 -> v1v2
		GColor = (v1v2.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v1v2 = MVP * v1v2;
		GNormal = VNormal[0];
        GPosition = vec3(v1v2);
        gl_Position = v1v2;
        EmitVertex();
        //p1 = v2
		//GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		//GNormal = VNormal[1];
  //      GPosition = VPosition[1];
  //      gl_Position = gl_in[1].gl_Position;
  //      EmitVertex();
        //p2 -> v1v3
		GColor = (v1v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v1v3 = MVP * v1v3;
		GNormal = VNormal[2];
        GPosition = vec3(v1v3);
        gl_Position = v1v3;
        EmitVertex();
    }

	else if(v1v2b && v2v3b){
        //p0 -> v1v2
		GColor = (v1v2.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v1v2 = MVP * v1v2;
		GNormal = VNormal[0];
        GPosition = vec3(v1v2);
        gl_Position = v1v2;
        EmitVertex();
        //p1 = v2
		//GColor = (WorldPosition[1].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		//GNormal = VNormal[1];
  //      GPosition = VPosition[1];
  //      gl_Position = gl_in[1].gl_Position;
  //      EmitVertex();
        //p2 -> v2v3
		GColor = (v2v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v2v3 = MVP * v2v3;
		GNormal = VNormal[2];
        GPosition = vec3(v2v3);
        gl_Position = v2v3;
        EmitVertex();
    }

		else if(v2v3b && v1v3b){
        //p0 -> v2v3
		GColor = (v2v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v2v3 = MVP * v2v3;
		GNormal = VNormal[0];
        GPosition = vec3(v2v3);
        gl_Position = v2v3;
        EmitVertex();
        //p1 = v3
		//GColor = (WorldPosition[2].y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		//GNormal = VNormal[2];
  //      GPosition = VPosition[2];
  //      gl_Position = gl_in[2].gl_Position;
  //      EmitVertex();
        //p2 -> v1v3
		GColor = (v1v3.y < h) ?  vec4( 1.0f,0.1f,0.0f, 1.0f ) : vec4( 0.0f,0.1f,1.0f, 1.0f );
		v1v3 = MVP * v1v3;
		GNormal = VNormal[2];
        GPosition = vec3(v1v3);
        gl_Position = v1v3;
        EmitVertex();
    }


	EndPrimitive();
}

