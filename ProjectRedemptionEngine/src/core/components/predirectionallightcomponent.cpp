#include <core/components/predirectionallightcomponent.h>

#include <glm/glm.hpp>

#include <list>
#include <set>

#include <include/modules/rendering.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

#include <core/subsystems/rendering/prerendering.h>

#ifdef __PRE_DEBUG__
#include <assert.h>
#endif

namespace PRE
{
	namespace Core
	{
		using std::list;
		using std::set;

		void PREDirectionalLightComponent::AddAffectingLightLayer(int lightLayer)
		{
#ifdef __PRE_DEBUG__
			assert(_affectingLightLayers.find(lightLayer) == _affectingLightLayers.end());
#endif
			_affectingLightLayers.insert(lightLayer);
		}

		void PREDirectionalLightComponent::RemoveAffectingLightLayer(int lightLayer)
		{
#ifdef __PRE_DEBUG__
			assert(_affectingLightLayers.find(lightLayer) != _affectingLightLayers.end());
#endif
			_affectingLightLayers.erase(lightLayer);
		}

		void PREDirectionalLightComponent::SetColor(const glm::vec3& color)
		{
			_color = color;
		}

		void PREDirectionalLightComponent::OnStart()
		{
			AllocateIfNotAllocated();
		}

		void PREDirectionalLightComponent::OnUpdate()
		{
		}

		void PREDirectionalLightComponent::OnDestroy()
		{
			// TODO: you can leak here if you manipulate and blow the object up
			// before the object has a chance to be added to the world
			DeallocateIfAllocated();
		}

		void PREDirectionalLightComponent::AllocateIfNotAllocated()
		{
			if (_pTransform == nullptr)
			{
				GetRendering().LinkLightToRenderTargets(*this);
				_pTransform = gameObject().GetComponent<PRETransformComponent>();
			}
		}

		void PREDirectionalLightComponent::DeallocateIfAllocated()
		{
			if (_pTransform != nullptr)
			{
				_pTransform = nullptr;
				GetRendering().UnlinkLightFromRenderTargets(*this);
			}
		}
	} // namespace Core
} // namespace PRE