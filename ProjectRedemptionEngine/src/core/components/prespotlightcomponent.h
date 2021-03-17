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

		class PRESpotLightComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetAttentuationLinear(float attenuationLinear);
			void SetAttentuationQuadratic(float attenuationQuadratic);
			// angle subtending center to radius of cone in degrees
			void SetSize(float innerRadius, float outerRadius);
			void SetColor(const glm::vec3& color);
			void SetDirection(const glm::vec3& direction);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			PRETransformComponent* _pTransform = nullptr;

			/*RenderCompositingNode* _pShadowNode = nullptr;
			RenderCompositingTarget* _pShadowTarget = nullptr;*/

			unordered_map<PRERenderTexture*, list<PRELightRenderPassData*>::iterator> _passMap;

			// numbers courtesy of ogre3d wiki
			float _attenuationLinear = 0.7f;
			float _attenuationQuadratic = 1.8f;
			float _innerRadius = glm::cos(glm::radians(12.5f));
			float _outerRadius = glm::cos(glm::radians(17.5f));
			glm::vec3 _color = glm::vec3(1.0f);
			glm::vec3 _direction = glm::vec3(0, 0, 1);

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE