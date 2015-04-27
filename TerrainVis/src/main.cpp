// template proect for opengl+glew+glfw
// if it shows a green screen, then it works!
// author: @tengel


//Include GLEW  
#include "GL/glew.h"

//Include GLFW  
#include <GLFW/glfw3.h>  

// GLM
#include <glm\glm.hpp>

//Include the standard C++ headers  
#include <iostream>
#include <cassert>

#include "TextureManager.h"

using namespace std;

mat4 g_Modelview = glm::mat4();
vec4 g_Middle(-0.5f, -0.5f, -0.5f, 1.0f);
vec3 g_Rotation(0, 0, 0);

//Define an error callback  
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}
	
//Define the key input callback  
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS){
		g_Middle += 0.1;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS){
		g_Middle -= 0.1;
	}
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS){
		g_Rotation.z = 1.0f;
	}
	else if (key == GLFW_KEY_X && action == GLFW_PRESS){
		g_Rotation.x = 1.0f;
	}
	else if (key == GLFW_KEY_C && action == GLFW_PRESS){
		g_Rotation.y = 1.0f;
	}
}

void mouseButtonCB(GLFWwindow* window, int btn, int action, int mods){
	// TODO
	cout << "btn " << action << endl;
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
	glMultMatrixf(&g_Modelview[0][0]);
	glTranslatef(g_Middle[0], g_Middle[1], g_Middle[2]);//Center the object!
	glRotatef(10, g_Rotation.x, g_Rotation.y, g_Rotation.z);
}

void SetupProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,1,0,1,0.1,100);
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
	window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);

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
		vec3 aux;
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

			//glBegin(GL_TRIANGLES);
			//glNormal3f(normals.at(i + 1).x, normals.at(i + 1).y, normals.at(i + 1).z);
			//aux = vertices.at(i * 3 + 3);
			//glVertex3f(aux.x, aux.y, aux.z);
			//aux = vertices.at(i * 3 + 4);
			//glVertex3f(aux.x, aux.y, aux.z);
			//aux = vertices.at(i * 3 + 5);
			//glVertex3f(aux.x, aux.y, aux.z);
			//glEnd();
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