
#ifndef VBOSPHERE_H
#define VBOSPHERE_H

#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "Scene.h"
#include <vector>


typedef struct
{
	GLfloat v[3]; //xyz
	GLfloat n[3]; //xyz
}vertex_t;


class VBOSphere : public Scene {
public:
	VBOSphere(int quality = 50);
	/**
	Load textures, initialize shaders, etc.
	*/
	void initScene();

	/**
	This is called prior to every frame.  Use this
	to update your animation.
	*/
	void update(double t);

	/**
	Draw your scene.
	*/
	void render();

	/**
	Called when screen is resized
	*/
	void resize(int, int);
private:
	void genVertex();
	void genBuffers();

	GLuint vaoID;
	int quality;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> colors;
	std::vector<unsigned int> indices;
};

#endif