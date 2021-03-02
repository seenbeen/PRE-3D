#pragma once
#include <include/modules/gameobject.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		class PRERendering;
		class PRETime;
		class PREWorld;

		class PREGameObjectTemplate;

		using GameObjectModule::GameObjectComponent;

		class PREGameObjectComponent : public GameObjectComponent
		{
			friend class PREGameObjectTemplate;

		protected:
			PRERendering* GetRendering();
			PRETime* GetTime();

			PREWorld* GetWorld();
			void Quit();

		private:
			PREApplicationContext* _preApplicationContext = nullptr;
		};
	} // namespace Core
} // namespace PRE