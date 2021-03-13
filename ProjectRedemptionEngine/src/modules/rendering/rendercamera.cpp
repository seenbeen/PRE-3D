#include <modules/rendering/rendercamera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		RenderCamera::Impl& RenderCamera::Impl::MakeImpl(
			const Kind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
		{
			return *(new Impl(kind, size, aspectRatio, nearClippingPlane, farClippingPlane));
		}

		RenderCamera::Impl::Impl(
			const Kind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
			:
			projectionMatrixChanged(true),
			kind(kind),
			size(size),
			aspectRatio(aspectRatio),
			nearClippingPlane(nearClippingPlane),
			farClippingPlane(farClippingPlane),
			viewMatrix(),
			projectionMatrix() {}

		RenderCamera::Impl::~Impl() {}

		void RenderCamera::SetKind(const RenderCamera::Kind& kind)
		{
			_impl.kind = kind;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetSize(float size)
		{
			_impl.size = size;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetAspectRatio(float aspectRatio)
		{
			_impl.aspectRatio = aspectRatio;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetNearClippingPlane(float nearClippingPlane)
		{
			_impl.nearClippingPlane = nearClippingPlane;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetFarClippingPlane(float farClippingPlane)
		{
			_impl.farClippingPlane = farClippingPlane;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetViewMatrix(const glm::mat4& viewMatrix)
		{
			_impl.viewMatrix = viewMatrix;
		}

		const glm::mat4& RenderCamera::GetViewMatrix()
		{

			return _impl.viewMatrix;
		}

		const glm::mat4& RenderCamera::GetProjectionMatrix()
		{
			if (_impl.projectionMatrixChanged)
			{
				_impl.projectionMatrixChanged = false;
				if (_impl.kind == Kind::ORTHOGRAPHIC)
				{
					auto hAspect = _impl.size * _impl.aspectRatio;
					_impl.projectionMatrix = glm::ortho(
						-hAspect,
						hAspect,
						-_impl.size,
						_impl.size,
						_impl.nearClippingPlane,
						_impl.farClippingPlane
					);
				}
				else
				{
					_impl.projectionMatrix = glm::perspective(
						glm::radians(_impl.size),
						_impl.aspectRatio,
						_impl.nearClippingPlane,
						_impl.farClippingPlane
					);
				}
			}
			return _impl.projectionMatrix;
		}

		RenderCamera::RenderCamera(
			const Kind& kind,
			float size,
			float aspectRatio,
			float nearClippingPlane,
			float farClippingPlane
		)
			:
			_impl(Impl::MakeImpl(kind, size, aspectRatio, nearClippingPlane, farClippingPlane)) {}

		RenderCamera::~RenderCamera()
		{
			delete &_impl;
		}
	} // namespace RenderingModule
} // namespace PRE