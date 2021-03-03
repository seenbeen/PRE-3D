#pragma once
#include <include/modules/gameobject.h>

namespace PRE
{
	namespace Core
	{
		struct PREWorldConfig;

		class PREApplicationContext;
		class PREGameObjectTemplate;

		using PRE::GameObjectModule::GameObjectWorld;
		using PRE::GameObjectModule::GameObject;
		using PRE::GameObjectModule::GameObjectComponent;

		class PREWorld
		{
			PREWorld& operator=(const PREWorld&) = delete;
			PREWorld(const PREWorld&) = delete;

			friend class PREApplicationContext;

		public:
			GameObject& Instantiate(PREGameObjectTemplate& preGameObjectTemplate);
			void Destroy(GameObject& gameObject);

		private:
			static PREWorld& MakePREWorld(
				const PREWorldConfig& worldConfig,
				PREApplicationContext& applicationContext
			);

			PREApplicationContext& _preApplicationContext;
			GameObjectWorld& _gameObjectWorld;

			PREWorld(
				PREApplicationContext& preApplicationContext,
				GameObjectWorld& gameObjectWorld
			);
			~PREWorld();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE