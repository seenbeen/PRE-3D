#include <modules/rendering/camera/camera.h>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		Camera::Camera()
			: transform(*this), _projectionMatrix() {}

		Camera::~Camera() {}

		glm::mat4 Camera::GetTransform()
		{
			return GetProjectionMatrix() * transform.GetInverseMatrix();
		}
	} // namespace RenderingModule
} // namespace PRE