#include "scenebasic.h"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glutils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Camera.h"

#define _DEBUG

using namespace std;
using std::ifstream;
using std::string;
using glm::mat4;
Camera* camera;

SceneBasic::SceneBasic(GLFWwindow* window) {
	this->window = window;
	camera = new Camera(window);
	
	// init matrices
	model = glm::mat4(1.0f);
	view = glm::lookAt(
		vec3(0.0f, 0.0f, -1.0f), //eye
		vec3(0.0f, 0.0f, 0.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
	mvpMat = glm::mat4(1.0f);
}

void SceneBasic::initScene()
{
#ifdef _DEBUG
	glDebugMessageCallback(GLUtils::debugCallback, NULL);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
		GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Start debugging");
#endif

	// load shaders
	try {
		prog.compileShader("shader/basic.vert");
		prog.compileShader("shader/basic.frag");
		prog.link();
		prog.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	prog.printActiveAttribs();
	//prog.setUniform("MVP", &mvpMat[0][0]);

	/////////////////// Create the VBO ////////////////////
	float positionData[] = {
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.0f, 0.8f, 0.0f };
	float colorData[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f };


	// Create and populate the buffer objects
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);  // Vertex position
	glEnableVertexAttribArray(1);  // Vertex color

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
}

void SceneBasic::setUpMatrices(){
	//model *= glm::translate(vec3(0.0f, 0.001f, 0.0f)); //push up
	//model *= glm::rotate(0.01f,vec3(1.0f, 1.0f, 0.0f)); //rotate xz
	view = camera->Look();
	model = glm::translate(vec3(0.0f, 0.0f, 1.0f)); // push back
	mvpMat = projection * view * model;
	prog.setUniform("ModelViewMatrix", mvpMat);
}

void SceneBasic::update(double deltaTime)
{
	handleInput();
	camera->Update(deltaTime);
	setUpMatrices();
	//...
}

void SceneBasic::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void SceneBasic::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

/**
	mouse and keyboard input
*/
void SceneBasic::handleInput(){
	
}