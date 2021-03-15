#include <core/components/prepointlightcomponent.h>

#include <list>
#include <unordered_map>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

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
		}

		void PREPointLightComponent::AllocateIfNotAllocated()
		{
		}

		void PREPointLightComponent::DeallocateIfAllocated()
		{
		}
	} // namespace Core
} // namespace PRE