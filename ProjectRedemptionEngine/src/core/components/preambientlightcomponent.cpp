#include <core/components/preambientlightcomponent.h>

#include <list>
#include <set>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		using std::list;

		void PREAmbientLightComponent::SetTag(int tag)
		{
			_tag = tag;
		}

		int PREAmbientLightComponent::GetTag()
		{
			return _tag;
		}

		void PREAmbientLightComponent::SetAttentuationLinear(float attenuationLinear)
		{
			_attenuationLinear = attenuationLinear;
		}

		void PREAmbientLightComponent::SetAttentuationQuadratic(float attenuationQuadratic)
		{
			_attenuationQuadratic = attenuationQuadratic;
		}

		void PREAmbientLightComponent::SetColor(const glm::vec3& color)
		{
			_color = color;
		}

		void PREAmbientLightComponent::OnStart()
		{
			AllocateIfNotAllocated();
		}

		void PREAmbientLightComponent::OnUpdate()
		{
		}

		void PREAmbientLightComponent::OnDestroy()
		{
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
			DeallocateIfAllocated();
		}

		void PREAmbientLightComponent::AllocateIfNotAllocated()
		{
			if (_pTransform == nullptr)
			{
				GetRendering().LinkLightToRenderTargets(*this);
				_pTransform = gameObject().GetComponent<PRETransformComponent>();
			}
		}

		void PREAmbientLightComponent::DeallocateIfAllocated()
		{
			if (_pTransform != nullptr)
			{
				_pTransform = nullptr;
				GetRendering().UnlinkLightFromRenderTargets(*this);
			}
		}
	} // namespace Core
} // namespace PRE