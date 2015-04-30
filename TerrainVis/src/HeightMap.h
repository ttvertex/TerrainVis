//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef HMAP_H
#define HMAP_H
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <GLFW\glfw3.h>
#include "FreeImage.h"
#include "glm\glm.hpp"
#include "glm\gtx\string_cast.hpp"
#include "Scene.h"

using namespace std;
using namespace glm;

typedef struct{
	vector<vec3>* vertices;
	vector<vec3>* normals;
	vector<GLuint>* index;
}Mesh;

class HeightMap : public Scene
{
public:
	HeightMap(GLFWwindow* window, const char* filename);
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

	Mesh* mesh = NULL;
	unsigned int height, width;
	GLuint vboID, vaoID, iboID;
	unsigned int ptr_inc;
	const char* filename;
};

#endif