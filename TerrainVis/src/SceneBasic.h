#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "scene.h"

#include "gl_core_4_4.h"
#include "glslprogram.h"
#include "GLFW\glfw3.h"

class SceneBasic : public Scene
{
private:
	int width, height;
	GLuint vboHandles[2];
	GLuint vaoHandle;
	GLSLProgram prog;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 mvpMat;

	void setUpMatrices();
	GLFWwindow* window;
public:
	SceneBasic(GLFWwindow* window);

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
	
};

#endif // SCENEBASIC_H
