#include <core/components/premodelrenderercomponent.h>

#include <set>
#include <vector>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>
#include <core/components/precameracomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>
#include <core/subsystems/rendering/prerendering.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace Core
	{
		using std::set;
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

		void PREModelRendererComponent::SetTag(int tag)
		{
			AllocateIfNotAllocated();
			GetRendering().UpdateModelRendererComponentTag(*this, tag);
		}

		int PREModelRendererComponent::GetTag()
		{
			return _tag;
		}

		void PREModelRendererComponent::SetCastsShadows(bool castsShadows)
		{
			_castsShadows = castsShadows;
		}

		bool PREModelRendererComponent::GetCastsShadows()
		{
			return _castsShadows;
		}

		void PREModelRendererComponent::OnStart()
		{
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
				_pTransformComponent = gameObject().GetComponent<PRETransformComponent>();
				_pModel = &GetRendering().AllocateModel();
				GetRendering().InitializeModelRendererComponentTag(*this);
			}
		}

		void PREModelRendererComponent::DeallocateIfAllocated()
		{
			if (_pModel != nullptr)
			{
				GetRendering().UninitializeModelRendererComponentTag(*this);
				GetRendering().DeallocateModel(*_pModel);
				_pModel = nullptr;
				_pTransformComponent = nullptr;
			}
		}
	} // namespace Core
} // namespace PRE