#include <core/components/prepointlightcomponent.h>

#include <list>

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

		void PREPointLightComponent::SetTag(int tag)
		{
			_tag = tag;
		}

		int PREPointLightComponent::GetTag()
		{
			return _tag;
		}

		void PREPointLightComponent::SetAttentuationLinear(float attenuationLinear)
		{
			_attenuationLinear = attenuationLinear;
		}

		void PREPointLightComponent::SetAttentuationQuadratic(float attenuationQuadratic)
		{
			_attenuationQuadratic = attenuationQuadratic;
		}

		void PREPointLightComponent::SetColor(const glm::vec3& color)
		{
			_color = color;
		}

		void PREPointLightComponent::OnStart()
		{
			AllocateIfNotAllocated();
		}

		void PREPointLightComponent::OnUpdate()
		{
		}

		void PREPointLightComponent::OnDestroy()
		{
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
			DeallocateIfAllocated();
		}

		void PREPointLightComponent::AllocateIfNotAllocated()
		{
			if (_pTransform == nullptr)
			{
				GetRendering().LinkLightToRenderTargets(*this);
				_pTransform = gameObject().GetComponent<PRETransformComponent>();
			}
		}

		void PREPointLightComponent::DeallocateIfAllocated()
		{
			if (_pTransform != nullptr)
			{
				_pTransform = nullptr;
				GetRendering().UnlinkLightFromRenderTargets(*this);
			}
		}
	} // namespace Core
} // namespace PRE