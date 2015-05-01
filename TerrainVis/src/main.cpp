#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "glutils.h"
#include "Scene.h"
#include "SceneBasic.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <gl\glew.h>

#include "HeightMap.h"

using std::string;
using namespace std;

Scene *scene;
GLFWwindow *window;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

void mouse_motion_callback(GLFWwindow* window, double x, double y){
	
}

void mouseButtonCB(GLFWwindow* window, int btn, int action, int mods){
	
}

void mouseScrollCB(GLFWwindow* window, double offx, double offy){
	
}

void initializeGL() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	scene->initScene();
}

void mainLoop() {
	double thisTime;
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		
		thisTime = glfwGetTime();
		scene->update(thisTime - lastTime);
		lastTime = thisTime;

		scene->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void resizeGL(int w, int h) {
	scene->resize(w, h);
}

int main(int argc, char *argv[])
{

	// Initialize GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);
	

	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	// Open the window
	string title = "TerrainVis by tengel";
	window = glfwCreateWindow(1000, 1000, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0, 0);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, GLFWcursorposfun(mouse_motion_callback));
	glfwSetMouseButtonCallback(window, GLFWmousebuttonfun(mouseButtonCB));
	glfwSetScrollCallback(window, GLFWscrollfun(mouseScrollCB));
	
	// Load the OpenGL functions.
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLUtils::dumpGLInfo();

	//scene = new SceneBasic(window);
	scene = new HeightMap(window, "resources/terrain2.jpg");

	// Initialization
	initializeGL();

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT_AND_BACK);

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit(EXIT_SUCCESS);
}