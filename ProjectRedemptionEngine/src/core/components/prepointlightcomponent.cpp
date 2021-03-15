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
		}

		void PREPointLightComponent::OnUpdate()
		{
		}

		void PREPointLightComponent::OnDestroy()
		{
		}

		void PREPointLightComponent::AllocateIfNotAllocated()
		{
		}
	} // namespace Core
} // namespace PRE