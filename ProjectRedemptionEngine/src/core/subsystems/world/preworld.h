#pragma once

namespace PRE
{
	namespace GameObjectModule
	{
		class GameObjectWorld;
		class GameObject;
		class GameObjectComponent;
	} // namespace GameObjectModule
} // namespace PRE

namespace PRE
{
	namespace Core
	{
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
			PREApplicationContext& _preApplicationContext;
			GameObjectWorld* const _gameObjectWorld;

			PREWorld(PREApplicationContext& preApplicationContext);
			~PREWorld();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE