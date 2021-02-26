#include <modules/rendering/camera/perspectivecamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		PerspectiveCamera::PerspectiveCamera(
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		) :
			_size(size),
			_aspectRatio(aspectRatio),
			_nearClippingPlane(nearClippingPlane),
			_farClippingPlane(farClippingPlane)
		{
			RefreshProjectionMatrix();
		}

		PerspectiveCamera::~PerspectiveCamera() {}

		float PerspectiveCamera::GetSize()
		{
			return _size;
		}

		void PerspectiveCamera::SetSize(float size)
		{
			_size = size;
			RefreshProjectionMatrix();
		}

		float PerspectiveCamera::GetAspectRatio()
		{
			return _aspectRatio;
		}

		void PerspectiveCamera::SetAspectRatio(float aspectRatio)
		{
			_aspectRatio = aspectRatio;
			RefreshProjectionMatrix();
		}

		float PerspectiveCamera::GetNearClippingPlane()
		{
			return _nearClippingPlane;
		}

		void PerspectiveCamera::SetNearClippingPlane(float nearClippingPlane)
		{
			_nearClippingPlane = nearClippingPlane;
			RefreshProjectionMatrix();
		}

		float PerspectiveCamera::GetFarClippingPlane()
		{
			return _farClippingPlane;
		}

		void PerspectiveCamera::SetFarClippingPlane(float farClippingPlane)
		{
			_farClippingPlane = farClippingPlane;
			RefreshProjectionMatrix();
		}

		void PerspectiveCamera::RefreshProjectionMatrix()
		{
			SetProjectionMatrix(
				glm::perspective(
					_size,
					_aspectRatio,
					_nearClippingPlane,
					_farClippingPlane
				)
			);
		}
	} // RenderingModule
} // PRE