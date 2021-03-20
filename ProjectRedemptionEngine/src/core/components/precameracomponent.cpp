#include <core/components/precameracomponent.h>

#include <vector>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>
#include <core/components/premodelrenderercomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/rendering/prerendertexture.h>
#include <core/subsystems/rendering/preskybox.h>

namespace PRE
{
	namespace Core
	{
		using std::vector;

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
			AllocateIfNotAllocated();
			if (_pRenderTexture != nullptr)
			{
				GetRendering().UnlinkCameraComponentFromRenderTexture(
					*this,
					*_pRenderTexture
				);
			}
			if (pRenderTexture != nullptr)
			{
				GetRendering().LinkCameraComponentToRenderTexture(
					*this,
					*pRenderTexture
				);
			}
		}

		PRERenderTexture* PRECameraComponent::GetRenderTexture() const
		{
			return _pRenderTexture;
		}

		void PRECameraComponent::SetSkyBox(PRESkyBox* pSkyBox)
		{
			AllocateIfNotAllocated();
			if (_pSkyBox != nullptr)
			{
				GetRendering().UnlinkCameraComponentFromSkyBox(
					*this,
					*_pSkyBox
				);
			}
			if (pSkyBox != nullptr)
			{
				GetRendering().LinkCameraComponentToSkyBox(
					*this,
					*pSkyBox
				);
			}
		}

		PRESkyBox* PRECameraComponent::GetSkyBox()
		{
			return _pSkyBox;
		}

		void PRECameraComponent::SetTag(int tag)
		{
			_tag = tag;
		}

		int PRECameraComponent::GetTag()
		{
			return _tag;
		}

		void PRECameraComponent::OnStart()
		{
			AllocateIfNotAllocated();
		}

		void PRECameraComponent::OnUpdate()
		{
			_pCamera->SetViewMatrix(
				_pTransformComponent->GetInverseMatrix()
			);

			if (_hasChanged)
			{
				_hasChanged = false;
				_pCamera->SetKind(
					_kind == PRECameraComponent::Kind::ORTHOGRAPHIC ?
					RenderCamera::Kind::ORTHOGRAPHIC :
					RenderCamera::Kind::PERSPECTIVE
				);
				_pCamera->SetSize(_size);
				_pCamera->SetAspectRatio(_aspectRatio);
				_pCamera->SetNearClippingPlane(_nearClippingPlane);
				_pCamera->SetFarClippingPlane(_farClippingPlane);
			}
		}

		void PRECameraComponent::OnDestroy()
		{
			DeallocateIfAllocated();
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
		}

		void PRECameraComponent::AllocateIfNotAllocated()
		{
			if (_pCamera == nullptr)
			{
				_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
				_pCamera = &GetRendering().AllocateCamera(
					_kind == Kind::ORTHOGRAPHIC ?
					PRERendering::CameraKind::ORTHOGRAPHIC :
					PRERendering::CameraKind::PERSPECTIVE,
					_size,
					_aspectRatio,
					_nearClippingPlane,
					_farClippingPlane
				);
			}
		}

		void PRECameraComponent::DeallocateIfAllocated()
		{
			if (_pCamera != nullptr)
			{
				if (_pRenderTexture != nullptr)
				{
					GetRendering().UnlinkCameraComponentFromRenderTexture(
						*this,
						*_pRenderTexture
					);
				}
				GetRendering().DeallocateCamera(*_pCamera);
				_pCamera = nullptr;
				_pTransformComponent = nullptr;
			}
		}
	} // namespace Core
} // namespace PRE