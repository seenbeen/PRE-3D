#include <core/components/premodelrenderercomponent.h>

#include <core/subsystems/rendering/prerendering.h>

#include <core/components/pretransformcomponent.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderModel;

		void PREModelRendererComponent::SetCameraComponent(PRECameraComponent* pCameraComponent)
		{
			_pCameraComponent = pCameraComponent;
			// hasChanged does not need to be set here.
		}

		PRECameraComponent* PREModelRendererComponent::GetCameraComponent() const
		{
			return _pCameraComponent;
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

		void PREModelRendererComponent::OnStart()
		{
			_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
			GetRendering().AllocateModel(*this);
		}

		void PREModelRendererComponent::OnUpdate()
		{
			GetRendering().UpdateModel(*this);
		}

		void PREModelRendererComponent::OnDestroy()
		{
			GetRendering().DeallocateModel(*this);
		}
	} // namespace Core
} // namespace PRE