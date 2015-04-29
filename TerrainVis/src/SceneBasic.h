#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "scene.h"

#include "gl_core_4_4.h"
#include "glslprogram.h"

class SceneBasic : public Scene
{
private:
	int width, height;
	GLuint vboHandles[2];
	GLuint vaoHandle;
	GLSLProgram prog;

public:
	SceneBasic();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
	
};

#endif // SCENEBASIC_H
