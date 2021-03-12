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

		void PREModelRendererComponent::SetCameraComponent(PRECameraComponent* pCameraComponent)
		{
			AllocateIfNotAllocated();
			if (_pCameraComponent != nullptr)
			{
				GetRendering().UnlinkModelRendererComponentFromCameraComponent(
					*this,
					*_pCameraComponent
				);
			}
			if (pCameraComponent != nullptr)
			{
				GetRendering().LinkModelRendererComponentToCameraComponent(
					*this,
					*pCameraComponent
				);
			}
			// hasChanged does not need to be set here.
		}

		PRECameraComponent* PREModelRendererComponent::GetCameraComponent() const
		{
			return _pCameraComponent;
		}

		void PREModelRendererComponent::OnStart()
		{
			_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
			AllocateIfNotAllocated();
		}

		void PREModelRendererComponent::OnUpdate()
		{
			_pModel->modelMatrix = _pTransformComponent->GetMatrix();

			if (_hasChanged)
			{
				_hasChanged = false;

				GetRendering().UpdateModelRendererComponentModel(*this);
			}
		}

		void PREModelRendererComponent::OnDestroy()
		{
			if (_pCameraComponent != nullptr)
			{
				GetRendering().UnlinkModelRendererComponentFromCameraComponent(
					*this,
					*_pCameraComponent
				);
			}
			GetRendering().DeallocateModel(*_pModel);
		}

		void PREModelRendererComponent::AllocateIfNotAllocated()
		{
			if (_pModel == nullptr)
			{
				_pModel = &GetRendering().AllocateModel();
			}
		}
	} // namespace Core
} // namespace PRE