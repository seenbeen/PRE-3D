#include <modules/rendering/camera/perspectivecamera.h>

#include <glm/glm.hpp>

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
			_farClippingPlane(farClippingPlane),
			_projectionMatrix()
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

		const glm::mat4& PerspectiveCamera::GetProjectionMatrix()
		{
			return _projectionMatrix;
		}

		void PerspectiveCamera::RefreshProjectionMatrix()
		{
			_projectionMatrix = glm::perspective(
				_size,
				_aspectRatio,
				_nearClippingPlane,
				_farClippingPlane
			);
		}
	} // RenderingModule
} // PRE