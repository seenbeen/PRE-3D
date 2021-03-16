#pragma once
#include <list>
#include <unordered_map>

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

		class PREPointLightComponent : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			void SetLuminosity(float luminosity);
			void SetColor(const glm::vec3& color);

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			bool initialized = false;
			/*RenderCompositingNode* _pShadowNode = nullptr;
			RenderCompositingTarget* _pShadowTarget = nullptr;*/

			unordered_map<PRERenderTexture*, list<PRELightRenderPassData*>::iterator> _passMap;

			float _luminosity = 1.0f;
			glm::vec3 _color = glm::vec3(1.0f);

			void AllocateIfNotAllocated();
			void DeallocateIfAllocated();
		};
	} // namespace Core
} // namespace PRE