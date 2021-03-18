#pragma once
#include <list>
#include <set>
#include <unordered_map>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRETransformComponent;
		class PRERendering;
		class PRELightRenderPassData;

		using std::list;
		using std::set;
		using std::unordered_map;

		class PREDirectionalLightComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void AddAffectingLightLayer(int lightLayer);
			void RemoveAffectingLightLayer(int lightLayer);

			void SetColor(const glm::vec3& color);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PRETransformComponent* _pTransform = nullptr;
			unordered_map<int, list<PRELightRenderPassData*>::iterator> _fronts;

			set<int> _affectingLightLayers;

			glm::vec3 _color = glm::vec3(1.0f);

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE