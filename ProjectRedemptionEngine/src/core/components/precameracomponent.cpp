#include <core/components/precameracomponent.h>

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
			_pNextRenderTexture = pRenderTexture;
			// hasChanged does not need to be set here.
		}

		PRERenderTexture* PRECameraComponent::GetRenderTexture() const
		{
			return _pNextRenderTexture;
		}

		void PRECameraComponent::SetSkyBox(const PRESkyBox* pSkyBox)
		{
			_pNextSkyBox = pSkyBox;
			// hasChanged does not need to be set here.
		}

		const PRESkyBox* PRECameraComponent::GetSkyBox()
		{
			return _pNextSkyBox;
		}

		void PRECameraComponent::OnStart()
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

			if (_pCurrentRenderTexture != _pNextRenderTexture)
			{
				if (_pCurrentRenderTexture != nullptr)
				{
					GetRendering().UnlinkCameraComponentFromRenderTexture(
						*this,
						*_pCurrentRenderTexture
					);
				}

				_pCurrentRenderTexture = _pNextRenderTexture;

				if (_pCurrentRenderTexture != nullptr)
				{
					GetRendering().LinkCameraComponentToRenderTexture(
						*this,
						*_pCurrentRenderTexture
					);
				}
			}

			if (_pCurrentSkyBox != _pNextSkyBox)
			{
				if (_pCurrentSkyBox != nullptr)
				{
					GetRendering().UnlinkSkyBoxFromCameraComponent(
						*_pCurrentSkyBox,
						*this
					);
				}

				_pCurrentSkyBox = _pNextSkyBox;

				if (_pCurrentSkyBox != nullptr)
				{
					GetRendering().LinkSkyBoxToCameraComponent(
						*_pCurrentSkyBox,
						*this
					);
				}
			}
		}

		void PRECameraComponent::OnDestroy()
		{
			if (_pCurrentRenderTexture != nullptr)
			{
				GetRendering().UnlinkCameraComponentFromRenderTexture(
					*this,
					*_pCurrentRenderTexture
				);

				for (
					auto it = _associatedModelComponents.begin();
					it != _associatedModelComponents.end();
					++it
					)
				{
					GetRendering().UnlinkModelRendererComponentFromCameraComponent(
						**it,
						*this
					);
				}

				_pCurrentRenderTexture = nullptr;
			}
			GetRendering().DeallocateCamera(*_pCamera);
		}
	} // namespace Core
} // namespace PRE