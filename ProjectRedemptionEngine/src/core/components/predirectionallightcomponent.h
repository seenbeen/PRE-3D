#pragma once
#include <list>
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
		class PREShadowRenderPassData;

		using std::list;
		using std::unordered_map;

		class PREDirectionalLightComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetTag(int tag);
			int GetTag();

			void SetColor(const glm::vec3& color);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PRETransformComponent* _pTransform = nullptr;
			unordered_map<int, list<PRELightRenderPassData*>::iterator> _fronts;

			unordered_map<int, PREShadowRenderPassData*> _shadowPassData;

			int _tag = 0;

			glm::vec3 _color = glm::vec3(1.0f);

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE