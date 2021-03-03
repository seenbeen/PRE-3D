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
			viewMatrixChanged(true),
			kind(kind),
			size(size),
			aspectRatio(aspectRatio),
			nearClippingPlane(nearClippingPlane),
			farClippingPlane(farClippingPlane),
			viewMatrix(),
			projectionMatrix(),
			viewProjectionMatrix() {}

		RenderCamera::Impl::~Impl() {}

		const RenderCamera::Kind& RenderCamera::GetKind() const {
			return _impl.kind;
		}

		void RenderCamera::SetKind(const RenderCamera::Kind& kind)
		{
			_impl.kind = kind;
			_impl.projectionMatrixChanged = true;
		}

		float RenderCamera::GetSize() const
		{
			return _impl.size;
		}

		void RenderCamera::SetSize(float size)
		{
			_impl.size = size;
			_impl.projectionMatrixChanged = true;
		}

		float RenderCamera::GetAspectRatio() const
		{
			return _impl.aspectRatio;
		}

		void RenderCamera::SetAspectRatio(float aspectRatio)
		{
			_impl.aspectRatio = aspectRatio;
			_impl.projectionMatrixChanged = true;
		}

		float RenderCamera::GetNearClippingPlane() const
		{
			return _impl.nearClippingPlane;
		}

		void RenderCamera::SetNearClippingPlane(float nearClippingPlane)
		{
			_impl.nearClippingPlane = nearClippingPlane;
			_impl.projectionMatrixChanged = true;
		}

		float RenderCamera::GetFarClippingPlane() const
		{
			return _impl.farClippingPlane;
		}

		void RenderCamera::SetFarClippingPlane(float farClippingPlane)
		{
			_impl.farClippingPlane = farClippingPlane;
			_impl.projectionMatrixChanged = true;
		}

		void RenderCamera::SetViewMatrix(const glm::mat4& viewMatrix)
		{
			_impl.viewMatrix = viewMatrix;
			_impl.viewMatrixChanged = true;
		}

		const glm::mat4& RenderCamera::GetViewProjectionMatrix()
		{
			if (_impl.viewMatrixChanged || _impl.projectionMatrixChanged)
			{
				_impl.viewMatrixChanged = false;
				if (_impl.projectionMatrixChanged)
				{
					_impl.projectionMatrixChanged = false;
					if (_impl.kind == Kind::ORTHOGRAPHIC)
					{
						auto hAspect = _impl.size * _impl.aspectRatio;
						_impl.projectionMatrix = glm::ortho(
							-_impl.size,
							_impl.size,
							-hAspect,
							hAspect,
							_impl.nearClippingPlane,
							_impl.farClippingPlane
						);
					}
					else
					{
						_impl.projectionMatrix = glm::perspective(
							_impl.size,
							_impl.aspectRatio,
							_impl.nearClippingPlane,
							_impl.farClippingPlane
						);
					}
				}
				_impl.viewProjectionMatrix = _impl.projectionMatrix * _impl.viewMatrix;
			}
			return _impl.viewProjectionMatrix;
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