#pragma once
#include <core/subsystems/world/pregameobjectcomponent.h>
#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderMesh;
		using PRE::RenderingModule::RenderMaterial;
		using PRE::RenderingModule::RenderModel;

		class PREModelRendererComponent : public PREGameObjectComponent
		{
		public:
			

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;
		};
	} // namespace Core
} // namespace PRE