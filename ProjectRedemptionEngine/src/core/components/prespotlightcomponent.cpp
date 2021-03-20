#include <core/components/prespotlightcomponent.h>

#include <glm/glm.hpp>

#include <list>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		using std::list;

		void PRESpotLightComponent::SetTag(int tag)
		{
			_tag = tag;
		}

		int PRESpotLightComponent::GetTag()
		{
			return _tag;
		}

		void PRESpotLightComponent::SetAttentuationLinear(float attenuationLinear)
		{
			_attenuationLinear = attenuationLinear;
		}

		void PRESpotLightComponent::SetAttentuationQuadratic(float attenuationQuadratic)
		{
			_attenuationQuadratic = attenuationQuadratic;
		}

		void PRESpotLightComponent::SetSize(float innerRadius, float outerRadius)
		{
			_innerRadius = glm::cos(glm::radians(innerRadius));
			_outerRadius = glm::cos(glm::radians(outerRadius));
		}

		void PRESpotLightComponent::SetColor(const glm::vec3& color)
		{
			_color = color;
		}

		void PRESpotLightComponent::OnStart()
		{
			AllocateIfNotAllocated();
		}

		void PRESpotLightComponent::OnUpdate()
		{
		}

		void PRESpotLightComponent::OnDestroy()
		{
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
			DeallocateIfAllocated();
		}

		void PRESpotLightComponent::AllocateIfNotAllocated()
		{
			if (_pTransform == nullptr)
			{
				GetRendering().LinkLightToRenderTargets(*this);
				_pTransform = gameObject().GetComponent<PRETransformComponent>();
			}
		}

		void PRESpotLightComponent::DeallocateIfAllocated()
		{
			if (_pTransform != nullptr)
			{
				_pTransform = nullptr;
				GetRendering().UnlinkLightFromRenderTargets(*this);
			}
		}
	} // namespace Core
} // namespace PRE