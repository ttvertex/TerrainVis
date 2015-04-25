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

	if (TextureManager::Inst()->LoadTexture("resources\\terrain.jpg", 1)){
		cout << "Image loaded!" << endl;
	}
	else{
		cout << "Problem loading image!" << endl;
	}

	//Main Loop  
	do
	{
		float side = 1.0f;
		//Clear color buffer  
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_TEXTURE_2D);
		
		if (!TextureManager::Inst()->BindTexture(1)){
			cout << "Error binding!" << endl;
		}

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-side , -side);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(side, -side);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(side, side);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(-side, side); 
		glEnd();

		glDisable(GL_TEXTURE_2D);

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