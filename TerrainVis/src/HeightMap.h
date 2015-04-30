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
#include <GL/glew.h>
#include <gl/gl.h>
#include <map>
#include "FreeImage.h"
#include "glm\glm.hpp"
#include "glm\gtx\string_cast.hpp"

using namespace std;
using namespace glm;

typedef struct{
	vector<vec3>* vertices;
	vector<vec3>* normals;
	vector<unsigned int>* index;
}Mesh;

typedef struct{
	glm::vec3 v;
	glm::vec3 n;
}vertex_t;

class HeightMap
{
public:
	HeightMap(const char* filename);

	//free the mesh
	bool unloadHMap(const unsigned int texID);

	//render the mesh
	bool renderMesh(const unsigned int texID);

	Mesh* getMesh();
	vector<vertex_t>* getVertices();
	void render();
private:
	vector<vertex_t>* vert_t;
	void genMesh(BYTE* imgData);
	void genBuffers();

	Mesh* mesh = NULL;
	unsigned int height, width;
	GLuint vboID, vaoID, iboID;
	unsigned int ptr_inc;
};

#endif