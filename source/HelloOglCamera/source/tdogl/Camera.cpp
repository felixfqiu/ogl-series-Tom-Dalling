
#include "Camera.h"
using namespace tdogl;

#include <glm/gtc/matrix_transform.hpp>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

Camera::Camera() :
	_pos(0.0f, 0.0f, 1.0f),
	_horAngle(0.0f),
	_verAngle(0.0f),
	_fieldOfView(50.0f),
	_nearPlane(0.01f),
	_farPlane(100.0f),
	_viewportAspectRatio(4.0f/3.0f)
{
}


const glm::vec3& Camera::pos() const
{
	return _pos;
}

void Camera::setPos(const glm::vec3& pos)
{
	_pos = pos;
}

glm::mat4 Camera::orientation() const
{
	glm::mat4 orientation;
	orientation = glm::rotate(orientation, glm::radians(_verAngle), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(_horAngle), glm::vec3(0, 1, 0));
	return orientation;
}

void Camera::lookAt(glm::vec3 target)
{
	assert(target != _pos);
	glm::vec3 direction = glm::normalize(target - _pos);
	_verAngle = glm::radians(asinf(-direction.y));
	_horAngle = glm::radians(atan2f(-direction.x, -direction.z));

	normalizeAngles(_horAngle, _verAngle, MaxVerticalAngle);
}

void Camera::normalizeAngles(float& horAngle, float& verAngle, float verAngleMax)
{
	horAngle = fmodf(horAngle, 360.0f);
	if (horAngle < 0.0f)
	{
		horAngle += 360.0f;
	}

	if (verAngle > verAngleMax)
	{
		verAngle = verAngleMax;
	}
	else if (verAngle < -verAngleMax)
	{
		verAngle = -verAngleMax;
	}
}

void Camera::offsetOrientation(float upAngle, float rightAngle)
{
	_horAngle += rightAngle;
	_verAngle += upAngle;
	normalizeAngles(_horAngle, _verAngle, MaxVerticalAngle);
}

float Camera::fieldOfView() const
{
	return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView)
{
	_fieldOfView = fieldOfView;
}

float Camera::nearPlane() const
{
	return _nearPlane;
}

float Camera::farPlane() const
{
	return _farPlane;
}

void Camera::setNearAndFarPlane(float nearPlane, float farPlane)
{
	_nearPlane = nearPlane;
	_farPlane = farPlane;
}

float Camera::viewportAspectRatio() const
{
	return _viewportAspectRatio;
}

void Camera::setViewportAspectRatio(float viewportAspectRatio)
{
	_viewportAspectRatio = viewportAspectRatio;
}

glm::mat4 Camera::matrix() const
{
	return projection() * view();
}

glm::mat4 Camera::projection() const
{
	return glm::perspective(glm::radians(_fieldOfView), _viewportAspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Camera::view() const
{
	return orientation() * glm::translate(glm::mat4(), -_pos);
}

void Camera::move(const glm::vec3& offset)
{
	_pos += offset;
}

glm::vec3 Camera::directionFacing() const
{
	glm::vec4 facing = glm::inverse(orientation()) * glm::vec4(0, 0, -1, -1);
	return glm::vec3(facing);
}

glm::vec3 Camera::directionRight() const
{
	glm::vec4 right = glm::inverse(orientation()) * glm::vec4(1, 0, 0, 1);
	return glm::vec3(right);
}

glm::vec3 Camera::directionUp() const
{
	glm::vec4 up = glm::inverse(orientation()) * glm::vec4(0, 1, 0, 1);
	return glm::vec3(up);
}