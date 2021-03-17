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
		class PRERenderTexture;
		class PRELightRenderPassData;

		using std::list;
		using std::unordered_map;

		using PRE::RenderingModule::RenderCompositingTarget;
		using PRE::RenderingModule::RenderCompositingNode;

		class PREAmbientLightComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetAttentuationLinear(float attenuationLinear);
			void SetAttentuationQuadratic(float attenuationQuadratic);
			void SetColor(const glm::vec3& color);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PRETransformComponent* _pTransform = nullptr;
			unordered_map<PRERenderTexture*, list<PRELightRenderPassData*>::iterator> _passMap;

			// numbers courtesy of ogre3d wiki
			float _attenuationLinear = 0.7f;
			float _attenuationQuadratic = 1.8f;

			glm::vec3 _color = glm::vec3(1.0f);

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE