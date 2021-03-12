#include <core/components/premodelrenderercomponent.h>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>
#include <core/components/precameracomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>
#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderModel;

		void PREModelRendererComponent::SetCameraComponent(PRECameraComponent* pCameraComponent)
		{
			_pNextCameraComponent = pCameraComponent;
			// hasChanged does not need to be set here.
		}

		PRECameraComponent* PREModelRendererComponent::GetCameraComponent() const
		{
			return _pNextCameraComponent;
		}

		void PREModelRendererComponent::SetMaterial(PREMaterial* pMaterial)
		{
			_pMaterial = pMaterial;
			_hasChanged = true;
		}

		PREMaterial* PREModelRendererComponent::GetMaterial() const
		{
			return _pMaterial;
		}

		void PREModelRendererComponent::SetMesh(PREMesh* pMesh)
		{
			_pMesh = pMesh;
			_hasChanged = true;
		}

		PREMesh* PREModelRendererComponent::GetMesh() const
		{
			return _pMesh;
		}

		void PREModelRendererComponent::SetSkeleton(PRESkeleton* pSkeleton)
		{
			_pSkeleton = pSkeleton;
			_hasChanged = true;
		}

		PRESkeleton* PREModelRendererComponent::GetSkeleton() const
		{
			return _pSkeleton;
		}

		void PREModelRendererComponent::OnStart()
		{
			_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
			_pModel = &GetRendering().AllocateModel();
		}

		void PREModelRendererComponent::OnUpdate()
		{
			_pModel->modelMatrix = _pTransformComponent->GetMatrix();

			if (_hasChanged)
			{
				_hasChanged = false;

				GetRendering().UpdateModelRendererComponentModel(*this);
			}

			if (_pCurrentCameraComponent != _pNextCameraComponent)
			{
				if (_pCurrentCameraComponent != nullptr)
				{
					GetRendering().UnlinkModelRendererComponentFromCameraComponent(
						*this,
						*_pCurrentCameraComponent
					);
				}

				_pCurrentCameraComponent = _pNextCameraComponent;

				if (_pCurrentCameraComponent != nullptr)
				{
					GetRendering().LinkModelRendererComponentToCameraComponent(
						*this,
						*_pCurrentCameraComponent
					);
				}
			}
		}

		void PREModelRendererComponent::OnDestroy()
		{
			if (_pCurrentCameraComponent != nullptr)
			{
				GetRendering().UnlinkModelRendererComponentFromCameraComponent(
					*this,
					*_pCurrentCameraComponent
				);

				_pCurrentCameraComponent = nullptr;
			}
			GetRendering().DeallocateModel(*_pModel);
		}
	} // namespace Core
} // namespace PRE