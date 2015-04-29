// template proect for opengl+glew+glfw
// if it shows a green screen, then it works!
// author: @tengel
#define WIDTH 1000
#define HEIGHT 1000

//Include GLEW  
#include "GL/glew.h"

//Include GLFW  
#include <GLFW/glfw3.h>  

// GLM
#include <glm\glm.hpp>
#include "glm\gtx\string_cast.hpp"
#include "glm\matrix.hpp"

//Include the standard C++ headers  
#include <iostream>
#include <cassert>

#include "HeightMap.h"

using namespace std;

mat4 g_Modelview = glm::mat4();
mat4 g_CameraMatrix = mat4();

vec4 g_Center = vec4(0.0f,0.0f,0.0f,1.0f);
vec4 g_Up	  = vec4(1.0f, 0.0f, 0.0f, 1.0f);
vec4 g_Eye    = vec4(0.0f, -1.0f, 0.0f, 1.0f);
bool mousePressed = false;
vec2 mousePos = vec2();

//Define an error callback  
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
	
//Define the key input callback  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT){
		switch (key){
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_W:
			g_Center.x += 0.5;
			break;
		case GLFW_KEY_S:
			g_Center.x -= 0.5;
			break;
		case GLFW_KEY_A:
			g_Center.z += 0.5;
			break;
		case GLFW_KEY_D:
			g_Center.z -= 0.5;
			break;
		}
	}
}
void cursorPosCB(GLFWwindow* window, double x, double y)
{
	vec3 axis(sqrt((float)((x - mousePos.x)*(x - mousePos.x) + (y - mousePos.y)*(y - mousePos.y))), (float)(y - mousePos.y), (float)(x - mousePos.x));
	//g_CameraMatrix = glm::rotate(mat4(), 10.0f, axis);
}

void mouseButtonCB(GLFWwindow* window, int btn, int action, int mods){

}

void mouseScrollCB(GLFWwindow* window, double offx, double offy){
	
	if (offy == 1){
		g_Center.y -= 0.5;
	}
	if (offy == -1){
		g_Center.y += 0.5;
	}
}

void SetupLighting()
{
	float ambiant[4] = { 0.2, 0.2, 0.2, 1 };
	float diffuse[4] = { 0.7, 0.7, 0.7, 1 };
	float specular[4] = { 1, 1, 1, 1 };
	float exponent = 4;
	float lightDir[4] = { -0.5f, 0.0f, -0.5f, 0 }; //{ g_Eye.x, g_Eye.y, g_Eye.z };

	glEnable(GL_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FRONT_AND_BACK);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiant);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	glEnable(GL_LIGHT0);
}

void SetupModelview()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(g_Center.x, g_Center.y, g_Center.z);
}

void SetupProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1, 0.01, 500);

}

int main(void)
{
	//Set the error callback  
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW  
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Declare a window object  
	GLFWwindow* window;

	//Create a window and create its OpenGL context  
	window = glfwCreateWindow(WIDTH, HEIGHT, "TerrainVis", NULL, NULL);

	//If the window couldn't be created  
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.   
	glfwMakeContextCurrent(window);

	//Sets the key callback  
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseButtonCB);
	glfwSetCursorPosCallback(window, GLFWcursorposfun(cursorPosCB));
	glfwSetScrollCallback(window, GLFWscrollfun(mouseScrollCB));
	//Initialize GLEW  
	GLenum err = glewInit();

	//If GLEW hasn't initialized  
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return -1;
	}

	printf("%s\n", glGetString(GL_RENDERER));
	printf("GL %s\n", glGetString(GL_VERSION));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	//Set a background color  
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	SetupLighting();

	HeightMap* hmap = new HeightMap("resources/terrain.jpg");
	if (!hmap){
		exit(EXIT_FAILURE);
	}
	Mesh* mesh = hmap->getMesh();
	vector<vec3>* vertices = mesh->vertices;
	vector<vec3>* normals = mesh->normals;
	vector<int>* index = mesh->index;
	
	vector<vertex_t>* verts = hmap->getVertices();

	for (int i = 0; i < 10; i++){
		cout << endl;
		cout << index->at(i) << endl;
		cout << glm::to_string(vertices->at(i)) << endl;
		cout << glm::to_string(normals->at(i)) << endl;
	}

	cout << "triangles: " << index->size() / 3 << endl;
	//Main Loop  
	do
	{
		glClear(GL_COLOR_BUFFER_BIT);
		SetupModelview();
		SetupProjection();

		gluLookAt(g_Eye.x, g_Eye.y, g_Eye.z,
			g_Center.x, g_Center.y, g_Center.z,
			g_Up.x, g_Up.y, g_Up.z);

		vec3 aux;
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//int n = 0;
		//for (int i = 0; i < index->size(); i+=3){
		//	//cout << "i=" << i * 3+2 << endl;
		//	glBegin(GL_TRIANGLE_STRIP);
		//	glNormal3f(normals->at(n).x, normals->at(n).y, normals->at(n).z);		
		//	aux = vertices->at(index->at(i));
		//	glVertex3f(aux.x, aux.y, aux.z);
		//	aux = vertices->at(index->at(i+1));
		//	glVertex3f(aux.x, aux.y, aux.z);
		//	aux = vertices->at(index->at(i+2));
		//	glVertex3f(aux.x, aux.y, aux.z);
		//	glEnd();
		//}
		// with normals
		//for (int i = 0; i < index->size(); i+=3){
		//	int ind[] = { index->at(i), index->at(i + 1), index->at(i + 2)};
		//	glBegin(GL_TRIANGLES);

		//	aux = normals->at(ind[0]);
		//	glNormal3f(aux.x, aux.y, aux.z);
		//	aux = vertices->at(ind[0]);
		//	glVertex3f(aux.x, aux.y, aux.z);
		//	
		//	aux = normals->at(ind[1]);
		//	glNormal3f(aux.x, aux.y, aux.z);
		//	aux = vertices->at(ind[1]);
		//	glVertex3f(aux.x, aux.y, aux.z);

		//	aux = normals->at(ind[2]);
		//	glNormal3f(aux.x, aux.y, aux.z);
		//	aux = vertices->at(ind[2]);
		//	glVertex3f(aux.x, aux.y, aux.z);

		//	glEnd();
		//}

		//triangle_t
		for (int i = 0; i < index->size(); i+=3){
			glBegin(GL_TRIANGLES);
			int ind[] = { index->at(i), index->at(i + 1), index->at(i + 2)};
			vertex_t v = verts->at(ind[0]);
			glNormal3f(v.n.x, v.n.y, v.n.z);
			glVertex3f(v.v.x, v.v.y, v.v.z);
			v = verts->at(ind[1]);
			glNormal3f(v.n.x, v.n.y, v.n.z);
			glVertex3f(v.v.x, v.v.y, v.v.z);
			v = verts->at(ind[2]);
			glNormal3f(v.n.x, v.n.y, v.n.z);
			glVertex3f(v.v.x, v.v.y, v.v.z);
			glEnd();
		}
		//Swap buffers  
		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();

	} //Check if the ESC key had been pressed or if the window had been closed  
	while (!glfwWindowShouldClose(window));

	//Close OpenGL window and terminate GLFW  
	glfwDestroyWindow(window);
	//Finalize and clean up GLFW  
	glfwTerminate();

	system("pause");
	exit(EXIT_SUCCESS);
}