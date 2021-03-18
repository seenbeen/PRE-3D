#include <core/components/premodelrenderercomponent.h>

#include <vector>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>
#include <core/components/precameracomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>
#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		using std::vector;

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

		void PREModelRendererComponent::AddCameraComponent(PRECameraComponent& cameraComponent)
		{
			AllocateIfNotAllocated();
			GetRendering().LinkModelRendererComponentToCameraComponent(
				*this,
				cameraComponent
			);
		}

		void PREModelRendererComponent::RemoveCameraComponent(PRECameraComponent& cameraComponent)
		{
			AllocateIfNotAllocated();
			GetRendering().UnlinkModelRendererComponentFromCameraComponent(
				*this,
				cameraComponent
			);
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
			DeallocateIfAllocated();
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
		}

		void PREModelRendererComponent::AllocateIfNotAllocated()
		{
			if (_pModel == nullptr)
			{
				_pModel = &GetRendering().AllocateModel();
			}
		}

		void PREModelRendererComponent::DeallocateIfAllocated()
		{
			if (_pModel != nullptr)
			{
				// unlink mutates _pCameraComponents, so we'll need to make a temp copy
				vector<PRECameraComponent*> cameraComponents(
					_pCameraComponents.begin(),
					_pCameraComponents.end()
				);
				for (auto it = cameraComponents.begin(); it != cameraComponents.end(); ++it)
				{
					GetRendering().UnlinkModelRendererComponentFromCameraComponent(
						*this,
						**it
					);
				}
				GetRendering().DeallocateModel(*_pModel);
				_pModel = nullptr;
			}
		}
	} // namespace Core
} // namespace PRE