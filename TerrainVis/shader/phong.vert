

varying vec3 N;
varying vec3 v;

uniform mat4 ModelViewMatrix;
uniform mat4 NormalViewMatrix;

void main(void)  
{     
   v = vec3(ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
}
     