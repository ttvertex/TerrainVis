#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "glutils.h"
#include "Scene.h"
#include "SceneBasic.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using std::string;

Scene *scene;
GLFWwindow *window;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_REPEAT){
		
	}
}

void initializeGL() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	scene->initScene();
}

void mainLoop() {
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		scene->update(float(glfwGetTime()));
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
	window = glfwCreateWindow(500, 500, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	//glfwSetKeyCallback(window, key_callback);

	// Load the OpenGL functions.
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLUtils::dumpGLInfo();

	scene = new SceneBasic(window);

	// Initialization
	initializeGL();

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit(EXIT_SUCCESS);
}