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

//Include the standard C++ headers  
#include <iostream>
#include <cassert>

#include "TextureManager.h"

using namespace std;

mat4 g_Modelview = glm::mat4();

vec4 g_Center = vec4(0,10,0,1);
vec4 g_Up = vec4(1,0,0,1); // y is up
vec4 g_Eye = vec4(0, -1, 0, 1);

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

}

void mouseButtonCB(GLFWwindow* window, int btn, int action, int mods){

}

void mouseScrollCB(GLFWwindow* window, double offx, double offy){
	if (offy == 1){
		g_Center.y *= 1.2;
	}
	else if (offy == -1){
		g_Center.y *= 0.8;
	}
}

void SetupLighting()
{
	float ambiant[4] = { 0.2, 0.2, 0.2, 1. };
	float diffuse[4] = { 0.7, 0.7, 0.7, 1. };
	float specular[4] = { 1, 1, 1, 1. };
	float exponent = 4;
	float lightDir[4] = { 0, 0, 1, 0 };

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
}

void SetupProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1, 0.01, 200);

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

	if (TextureManager::Inst()->LoadTexture("resources/terrain.jpg", 1)){
		cout << "Image loaded!" << endl;
	}
	else{
		cout << "Problem loading image!" << endl;
	}

	heightmap_t mesh = TextureManager::Inst()->genMesh();
	vector<vec3> vertices = mesh.vertices;
	vector<vec3> normals = mesh.normals;

	for (int i = 0; i < 20; i++){
		cout << glm::to_string(vertices.at(i)) << endl;
	}

	//Main Loop  
	do
	{
		float side = 1.0f;
		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT);

		//glEnable(GL_TEXTURE_2D);
		//
		//if (!TextureManager::Inst()->BindTexture(1)){
		//	cout << "Error binding!" << endl;
		//}

		//glBegin(GL_QUADS);
		//glTexCoord2f(0.0f, 0.0f);
		//glVertex2f(-side , -side);
		//glTexCoord2f(1.0f, 0.0f);
		//glVertex2f(side, -side);
		//glTexCoord2f(1.0f, 1.0f);
		//glVertex2f(side, side);
		//glTexCoord2f(0.0f, 1.0f);
		//glVertex2f(-side, side); 
		//glEnd();

		//glDisable(GL_TEXTURE_2D);
		SetupModelview();
		SetupProjection();

		gluLookAt(g_Eye.x, g_Eye.y, g_Eye.z,
			g_Center.x, g_Center.y, g_Center.z,
			g_Up.x, g_Up.y, g_Up.z);

		vec3 aux;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPolygonMode(GL_FRONT, GL_FILL);

		for (int i = 0; i < normals.size() - 1; i++){
			//cout << "i=" << i * 3+2 << endl;
			glBegin(GL_TRIANGLES);
			glNormal3f(normals.at(i).x, normals.at(i).y, normals.at(i).z);
			aux = vertices.at(i * 3);
			glVertex3f(aux.x, aux.y, aux.z);
			aux = vertices.at(i * 3 + 1);
			glVertex3f(aux.x, aux.y, aux.z);
			aux = vertices.at(i * 3 + 2);
			glVertex3f(aux.x, aux.y, aux.z);
			glEnd();
		}
		//glBegin(GL_TRIANGLES);
		//glNormal3f(0, 0, -1);
		//glVertex3f(0.0f, 0.5f, 0.0f);
		//glVertex3f(0.0f, 0.0f, 0.5f);
		//glVertex3f(0.5f, 0.0f, 0.0f);
		//glEnd();


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