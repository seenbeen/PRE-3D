#include <modules/rendering/camera/camera.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		Camera::Camera()
			:
			_viewMatrix(),
			_projectionMatrix(),
			_viewProjectionMatrix() {}

		Camera::~Camera() {}

		void Camera::SetViewMatrix(const glm::mat4& viewMatrix)
		{
			_viewMatrix = viewMatrix;
			_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
		}

		const glm::mat4& Camera::GetViewProjectionMatrix() const
		{
			return _viewProjectionMatrix;
		}

		void Camera::SetProjectionMatrix(const glm::mat4& projectionMatrix)
		{
			_projectionMatrix = projectionMatrix;
			_viewProjectionMatrix = _projectionMatrix * _viewMatrix;
		}
	} // namespace RenderingModule
} // namespace PRE