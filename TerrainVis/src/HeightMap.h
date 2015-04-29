//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#ifndef HMAP_H
#define HMAP_H
#include <vector>
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include "FreeImage.h"
#include <map>
#include "glm\glm.hpp"

using namespace std;
using namespace glm;

typedef struct{
	vector<vec3>* vertices;
	vector<vec3>* normals;
	vector<int>* index;
}Mesh;

class HeightMap
{
public:
	HeightMap(const char* filename);

	//free the mesh
	bool unloadHMap(const unsigned int texID);

	//render the mesh
	bool renderMesh(const unsigned int texID);

	Mesh* getMesh();
	
private:
	void genMesh(BYTE* imgData);

	Mesh* mesh = NULL;
	unsigned int height, width;
	GLuint vboID, vaoID;
	unsigned int ptr_inc;
};

#endif