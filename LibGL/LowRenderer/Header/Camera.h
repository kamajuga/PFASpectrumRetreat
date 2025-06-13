#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#ifndef __gl_h_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif // !__gl_h_

#include "LibMath/Vector.h"
#include "LibMath/Matrix.h"
#include "LibMath/Trigonometry.h"

class Camera
{
public:
	Camera(float fov = static_cast<float>(M_PI) / 2.f, float aspectRatio = 10.f / 10.f, 
			float nearPlane = 0.1f, float farPlane = 100.f);
	Camera(const LibMath::Vector3& eye, const LibMath::Vector3& center, const LibMath::Vector3& up, 
		float fovYRadians, float aspect,
		float nearPlane, float farPlane);

	LibMath::Matrix4 createViewMatrix(LibMath::Vector3 position, float pitch, float yaw);
	LibMath::Matrix4 getViewMatrix() const { return m_viewMatrix; }
	LibMath::Matrix4 getProjectionMatrix() const { return m_projectionMatrix; }

	void setTransformMatrix(LibMath::Vector3 const translate = LibMath::Vector3(0.0f, 0.0f, 0.0f), // returns T . R . S
							LibMath::Radian const rotation = LibMath::Radian(0.0f),
							LibMath::Vector3 const scale = LibMath::Vector3(1.0f, 1.0f, 1.0f));

	LibMath::Matrix4 lookAt(const LibMath::Vector3& eye, const LibMath::Vector3& center, const LibMath::Vector3& up);
	
	void uploadPositionToGPU(GLuint shaderProgram, const std::string& uniformName);

	void updateCameraVectors();
	

	~Camera() = default;

	float m_fov;
	float m_aspectRatio;
	float m_nearPlane;
	float m_farPlane;
	float m_moveSpeed = 30.f;

	bool m_firstMouse = true;
	float   m_lastX = 0.0f;
	float   m_lastY = 0.0f;
	float	m_mouseSensitivity = 0.1f;

	LibMath::Vector3 m_position;
	LibMath::Vector3 m_direction;
	LibMath::Vector3 m_up;
	LibMath::Vector3 m_right;
	LibMath::Vector3 m_front;
	LibMath::Vector3 m_worldUp;
	LibMath::Radian		m_pitch;
	LibMath::Radian		m_yaw;
	LibMath::Matrix4 m_viewMatrix;
	LibMath::Matrix4 m_projectionMatrix;
	LibMath::Matrix4 m_transformMatrix;
private:
	
	


};