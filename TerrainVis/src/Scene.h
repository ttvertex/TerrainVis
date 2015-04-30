#ifndef SCENE_H
#define SCENE_H

class Scene
{
public:
	Scene() { }

	/**
	Load textures, initialize shaders, etc.
	*/
	virtual void initScene() = 0;

	/**
	This is called prior to every frame.  Use this
	to update your animation.
	*/
	virtual void update(float t) = 0;

	/**
	Draw your scene.
	*/
	virtual void render() = 0;

	/**
	Called when screen is resized
	*/
	virtual void resize(int, int) = 0;

	virtual void keyCallback(int key, int scancode, int action, int mods) = 0;
	virtual void mouseButtonCallback(int btn, int action, int mods) = 0;
	virtual void mouseScrollCallback(int offx, int offy) = 0;
	virtual void mouseMotionCallback(float x, float y) = 0;

protected:

};

#endif // SCENE_H
