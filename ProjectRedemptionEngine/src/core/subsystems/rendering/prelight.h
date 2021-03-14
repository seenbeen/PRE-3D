#pragma once
#include <include/modules/rendering.h>

#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;
		class PRETransformComponent;

		class PRELight : public PREGameObjectComponent
		{
			friend class PRERendering;

		public:
			enum class Kind { DIRECTIONAL, SPOT, POINT };

		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;
		
		private:

		};
	} // namespace Core
} // namespace PRE