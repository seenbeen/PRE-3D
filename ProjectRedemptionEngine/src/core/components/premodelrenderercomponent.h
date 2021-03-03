#pragma once
#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PREMaterial;
		class PREMesh;

		class PREModelRendererComponent : public PREGameObjectComponent
		{
		public:
			PREMaterial* material = nullptr;
			PREMesh* mesh = nullptr;

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;
		};
	} // namespace Core
} // namespace PRE