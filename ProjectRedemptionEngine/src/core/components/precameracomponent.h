#pragma once
#include <core/subsystems/world/pregameobjectcomponent.h>

namespace PRE
{
	namespace Core
	{
		class PRECameraComponent : public PREGameObjectComponent
		{
		public:


		protected:
			void OnStart() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:

		};
	} // namespace Core
} // namespace PRE