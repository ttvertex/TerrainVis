#pragma once
#include "gl_core_4_4.h"
#include "glslprogram.h"
#include "GLFW\glfw3.h"

class Camera
{
public:
	glm::vec3 vEye, vView, vUp;
	float fSpeed;
	float fSensitivity; // How many degrees to rotate per pixel moved by mouse (nice value is 0.10)

	// Main functions
	void RotateWithMouse();
	void Update(double deltaTime);
	glm::mat4 Look();

	void SetMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight);	

	// Functions that get viewing angles
	float GetAngleX(), GetAngleY();

	// Constructors
	Camera(GLFWwindow* window);
	Camera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity);

private:
	GLFWwindow* window;
	char iForw, iBack, iLeft, iRight;
	double mouseX, mouseY;
};