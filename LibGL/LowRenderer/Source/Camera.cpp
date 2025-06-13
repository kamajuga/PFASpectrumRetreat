#include "Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	// Initialize the projection matrix
	m_projectionMatrix = LibMath::Matrix4::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

Camera::Camera(const LibMath::Vector3& eye, const LibMath::Vector3& center, const LibMath::Vector3& up, float fovYRadians, float aspect, float nearPlane, float farPlane)
{
    m_fov = fovYRadians;
    m_aspectRatio = aspect;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
	m_position = eye;
	m_worldUp = up;
    LibMath::Vector3 dir = center - eye;
    dir.normalize();
    m_yaw = LibMath::atan(dir.getZ(), dir.getX());
    m_pitch = LibMath::asin(dir.getY());
    m_projectionMatrix = LibMath::Matrix4::perspective(fovYRadians, aspect, nearPlane, farPlane);
    updateCameraVectors();
    m_viewMatrix = lookAt(m_position, m_position + m_front, m_up);

}

LibMath::Matrix4 Camera::createViewMatrix(LibMath::Vector3 position, float pitch, float yaw)
{
    // I assume the values are already converted to radians.
    float cosPitch = LibMath::cos(LibMath::Radian(pitch));
    float sinPitch = LibMath::sin(LibMath::Radian(pitch));
    float cosYaw = LibMath::cos(LibMath::Radian(yaw));
    float sinYaw = LibMath::sin(LibMath::Radian(yaw));


    // Right vector (X-axis)
    LibMath::Vector3 xaxis = -LibMath::Vector3{ cosYaw, 0, -sinYaw };
    xaxis.normalize();


    // Forward vector (Z-axis, looking direction)
    LibMath::Vector3 zaxis = -LibMath::Vector3{ sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };
    zaxis.normalize();

    // The "up" vector(Y-axis).
    LibMath::Vector3 yaxis = xaxis.cross(zaxis);


    float dotXAxis = xaxis.dot(position);
    float dotYAxis = yaxis.dot(position);
    float dotZAxis = zaxis.dot(position);


    LibMath::Matrix4 ViewMatrix(
        xaxis.getX(), yaxis.getX(), zaxis.getX(), -dotXAxis,
        xaxis.getY(), yaxis.getY(), zaxis.getY(), -dotYAxis,
        xaxis.getZ(), yaxis.getZ(), zaxis.getZ(), -dotZAxis,
        0.f, 0.f, 0.f, 1
    );

    // Create a 4x4 view matrix from the right, up, forward and eye position vectors
    return ViewMatrix;
}

void Camera::setTransformMatrix(LibMath::Vector3 const translate, LibMath::Radian const rotation, LibMath::Vector3 const scale)
{
	m_transformMatrix = LibMath::Matrix4::createTransform(translate, rotation, scale);

}

LibMath::Matrix4 Camera::lookAt(const LibMath::Vector3& eye, const LibMath::Vector3& center, const LibMath::Vector3& up)
{
    LibMath::Vector3 f = (center - eye);
    f.normalize();
    LibMath::Vector3 s = f.cross(up);
    s.normalize();
    LibMath::Vector3 u = s.cross(f);

    LibMath::Matrix4 result = LibMath::Matrix4::identity();

    result[0][0] = s.getX(); result[1][0] = s.getY(); result[2][0] = s.getZ();
    result[0][1] = u.getX(); result[1][1] = u.getY(); result[2][1] = u.getZ();
    result[0][2] = -f.getX(); result[1][2] = -f.getY(); result[2][2] = -f.getZ();

    result[3][0] = -s.dot(eye);
    result[3][1] = -u.dot(eye);
    result[3][2] = f.dot(eye);

    return result;
}

void Camera::uploadPositionToGPU(GLuint shaderProgram, const std::string& uniformName)
{
	GLint location = glGetUniformLocation(shaderProgram, uniformName.c_str());
	if (location == -1) {
		std::cerr << "Warning: uniform '" << uniformName << "' not found in shader program." << std::endl;
		//throw std::runtime_error("Model matrix uniform not found: " + uniformName);
		return;
	}
	glUniform3f(location, m_position.getX(), m_position.getY(), m_position.getZ());
}

void Camera::updateCameraVectors()
{
    LibMath::Vector3 front;
    front.getX() = LibMath::cos(m_yaw) * LibMath::cos(m_pitch);
    front.getY() = LibMath::sin(m_pitch);
    front.getZ() = LibMath::sin(m_yaw) * LibMath::cos(m_pitch);
    front.normalize();

    m_front = front;
    m_right = m_front.cross(m_worldUp);
    m_right.normalize();
    m_up = m_right.cross(m_front);
    m_up.normalize();

	
}
