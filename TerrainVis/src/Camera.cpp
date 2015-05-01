#include "Camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
using namespace std;

const float PI = float(atan(1.0)*4.0);

Camera::Camera(GLFWwindow* window)
{
	this->window = window;
	vEye = glm::vec3(0.0f, 0.0f, 0.0f);
	vView = glm::vec3(0.0f, 0.0, 10.0f);
	vUp = glm::vec3(0.0f, 1.0f, 0.0f);
	fSpeed = 2.0f;
	fSensitivity = 0.01f;
	this->SetMovingKeys('W', 'S', 'A', 'D');
}

Camera::Camera(glm::vec3 a_vEye, glm::vec3 a_vView, glm::vec3 a_vUp, float a_fSpeed, float a_fSensitivity)
{
	vEye = a_vEye; vView = a_vView; vUp = a_vUp;
	fSpeed = a_fSpeed;
	fSensitivity = a_fSensitivity;
}

/*-----------------------------------------------

Name:	rotateWithMouse

Params:	none

Result:	Checks for moving of mouse and rotates
camera.

/*---------------------------------------------*/

void Camera::RotateWithMouse()
{
	int left, top, right, bottom;
	glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
	glfwGetCursorPos(window, &mouseX, &mouseY);
	
	float iCentX = (left + right) >> 1;
	float iCentY =  (top + bottom) >> 1;
	float deltaX = (iCentX - float(mouseX)) * fSensitivity;
	float deltaY = (iCentY - float(mouseY)) * fSensitivity;

	if (deltaX != 0.0f)
	{
		vView -= vEye;
		vView = glm::rotate(vView, deltaX, glm::vec3(0.0f, 1.0f, 0.0f));
		vView += vEye;
	}
	if (deltaY != 0.0f)
	{
		glm::vec3 vAxis = glm::cross(vView - vEye, vUp);
		vAxis = glm::normalize(vAxis);
		float fAngle = deltaY;
		float fNewAngle = fAngle + GetAngleX();
		if (fNewAngle > -89.80f && fNewAngle < 89.80f)
		{
			vView -= vEye;
			vView = glm::rotate(vView, deltaY, vAxis);
			vView += vEye;
		}
	}
	glfwSetCursorPos(window, iCentX, iCentY);
}

/*-----------------------------------------------

Name:	GetAngleY

Params:	none

Result:	Gets Y angle of camera (head turning left
and right).

/*---------------------------------------------*/

float Camera::GetAngleY()
{
	glm::vec3 vDir = vView - vEye; vDir.y = 0.0f;
	glm::normalize(vDir);
	float fAngle = acos(glm::dot(glm::vec3(0, 0, -1), vDir))*(180.0f / PI);
	if (vDir.x < 0)fAngle = 360.0f - fAngle;
	return fAngle;
}

/*-----------------------------------------------

Name:	GetAngleX

Params:	none

Result:	Gets X angle of camera (head turning up
and down).

/*---------------------------------------------*/

float Camera::GetAngleX()
{
	glm::vec3 vDir = vView - vEye;
	vDir = glm::normalize(vDir);
	glm::vec3 vDir2 = vDir; vDir2.y = 0.0f;
	vDir2 = glm::normalize(vDir2);
	float fAngle = acos(glm::dot(vDir2, vDir))*(180.0f / PI);
	if (vDir.y < 0)fAngle *= -1.0f;
	return fAngle;
}

/*-----------------------------------------------

Name:	SetMovingKeys

Params:	a_iForw - move forward Key
a_iBack - move backward Key
a_iLeft - strafe left Key
a_iRight - strafe right Key

Result:	Sets Keys for moving camera.

/*---------------------------------------------*/

void Camera::SetMovingKeys(int a_iForw, int a_iBack, int a_iLeft, int a_iRight)
{
	iForw = a_iForw;
	iBack = a_iBack;
	iLeft = a_iLeft;
	iRight = a_iRight;
}

/*-----------------------------------------------

Name:	Update

Params:	none

Result:	Performs updates of camera - moving and
rotating.

/*---------------------------------------------*/

void Camera::Update(double deltaTime)
{
	RotateWithMouse();

	// Get view direction
	glm::vec3 vMove = vView - vEye;
	vMove = glm::normalize(vMove);
	vMove *= fSpeed;

	glm::vec3 vStrafe = glm::cross(vView - vEye, vUp);
	vStrafe = glm::normalize(vStrafe);
	vStrafe *= fSpeed;

	int iMove = 0;
	glm::vec3 vMoveBy;
	 //Get vector of move
	if (glfwGetKey(window, iForw))
		vMoveBy += vMove * float(deltaTime);
	if (glfwGetKey(window, iBack))
		vMoveBy -= vMove * float(deltaTime);
	if (glfwGetKey(window, iLeft))
		vMoveBy -= vStrafe * float(deltaTime);
	if (glfwGetKey(window, iRight))
		vMoveBy += vStrafe * float(deltaTime);
	vEye += vMoveBy; 
	vView += vMoveBy;
}

/*-----------------------------------------------

Name:	Look

Params:	none

Result:	Returns proper modelview matrix to make
camera look.

/*---------------------------------------------*/

glm::mat4 Camera::Look()
{
	return glm::lookAt(vEye, vView, vUp);
}