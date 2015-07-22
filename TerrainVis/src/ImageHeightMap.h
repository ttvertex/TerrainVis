#ifndef HMAP_H
#define HMAP_H

#include "gl_core_4_4.h"
#include "glslprogram.h"
#include "GLFW\glfw3.h"
#include "FreeImage.h"
#include "Scene.h"
#include <vector>
#include "Camera.h"

using namespace std;
using namespace glm;

typedef struct{
	vector<vec3>* vertices;
	vector<vec3>* normals;
	vector<GLuint>* index;
}Mesh;

class ImageHeightMap : public Scene
{
public:
	ImageHeightMap(GLFWwindow* window, const char* filename);
	/**
	Load textures, initialize shaders, etc.
	*/
	void initScene();
	/**
	This is called prior to every frame.  Use this
	to update your animation.
	*/
	void update(double deltaTime);
	/**
	Draw your scene.
	*/
	void render();
	/**
	Called when screen is resized
	*/
	void resize(int, int);
	/*
	free the mesh
	*/
	bool unload();

private:
	void genMesh(BYTE* imgData);
	void genBuffers();
	void loadImage();
	void handleInput();

	GLSLProgram prog;
	GLSLProgram shaderProg;
	BYTE* rawImage;
	Mesh* mesh = NULL;
	unsigned int height, width;
	GLuint vboID, vaoID, iboID;
	unsigned int ptr_inc;
	const char* filename;
	
	Camera* camera;
	GLFWwindow* window;
	bool wireframe;


	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

#endif