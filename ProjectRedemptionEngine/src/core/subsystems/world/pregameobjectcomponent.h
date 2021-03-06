#pragma once
#include <include/modules/gameobject.h>

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;

		class PREInput;
		class PRERendering;
		class PRETime;
		class PREWorld;
		class PREAssetManager;

		class PREGameObjectTemplate;

		using GameObjectModule::GameObjectComponent;

		class PREGameObjectComponent : public GameObjectComponent
		{
			friend class PREGameObjectTemplate;

		protected:
			PREInput& GetInput();
			PRERendering& GetRendering();
			PRETime& GetTime();
			PREWorld& GetWorld();
			PREAssetManager& GetAssetManager();

			void Quit();

		private:
			PREApplicationContext* _preApplicationContext = nullptr;
		};
	} // namespace Core
} // namespace PRE