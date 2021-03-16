#include <core/components/prepointlightcomponent.h>

#include <list>
#include <unordered_map>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		void PREPointLightComponent::SetLuminosity(float luminosity)
		{
			_luminosity = luminosity;
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
			if (!initialized)
			{
				GetRendering().LinkLightToRenderTargets(*this);
				initialized = true;
			}
		}

		void PREPointLightComponent::DeallocateIfAllocated()
		{
			if (initialized)
			{
				GetRendering().UnlinkLightFromRenderTargets(*this);
				initialized = false;
			}
		}
	} // namespace Core
} // namespace PRE