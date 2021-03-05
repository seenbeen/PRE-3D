#include <core/components/precameracomponent.h>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prerendertexture.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::Renderer;
		using PRE::RenderingModule::RenderCamera;

		void PRECameraComponent::SetKind(PRECameraComponent::Kind kind)
		{
			_kind = kind;
			_hasChanged = true;
		}

		PRECameraComponent::Kind PRECameraComponent::Getkind() const
		{
			return _kind;
		}

		void PRECameraComponent::SetSize(float size)
		{
			_size = size;
			_hasChanged = true;
		}

		float PRECameraComponent::GetSize() const
		{
			return _size;
		}

		void PRECameraComponent::SetAspectRatio(float aspectRatio)
		{
			_aspectRatio = aspectRatio;
			_hasChanged = true;
		}

		float PRECameraComponent::GetAspectRatio() const
		{
			return _aspectRatio;
		}

		void PRECameraComponent::SetNearClippingPlane(float nearClippingPlane)
		{
			_nearClippingPlane = nearClippingPlane;
			_hasChanged = true;
		}

		float PRECameraComponent::GetNearClippingPlane() const
		{
			return _nearClippingPlane;
		}

		void PRECameraComponent::SetFarClippingPlane(float farClippingPlane)
		{
			_farClippingPlane = farClippingPlane;
			_hasChanged = true;
		}

		float PRECameraComponent::GetFarClippingPlane() const
		{
			return _farClippingPlane;
		}

		void PRECameraComponent::SetRenderTexture(PRERenderTexture* pRenderTexture)
		{
			_pRenderTexture = pRenderTexture;
			// hasChanged does not need to be set here.
		}

		PRERenderTexture* PRECameraComponent::GetRenderTexture() const
		{
			return _pRenderTexture;
		}

		void PRECameraComponent::OnStart()
		{
			_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
			GetRendering().AllocateCamera(*this);
		}

		void PRECameraComponent::OnUpdate()
		{
			GetRendering().UpdateCamera(*this);
		}

		void PRECameraComponent::OnDestroy()
		{
			GetRendering().DeallocateCamera(*this);
		}
	} // namespace Core
} // namespace PRE