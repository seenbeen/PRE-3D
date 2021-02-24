#pragma once

namespace PRE
{
	namespace GameObjectModule
	{
		class GameObjectWorld;
		class GameObjectTemplate;
		class GameObject;
		class GameObjectComponent;
	} // namespace GameObjectModule
} // namespace PRE

namespace PRE
{
	namespace Core
	{
		class PREApplicationContext;
		
		using PRE::GameObjectModule::GameObjectWorld;
		using PRE::GameObjectModule::GameObjectTemplate;
		using PRE::GameObjectModule::GameObject;
		using PRE::GameObjectModule::GameObjectComponent;

		class PREWorld
		{
			PREWorld& operator=(const PREWorld&) = delete;
			PREWorld(const PREWorld&) = delete;

			friend class PREApplicationContext;

		public:
			GameObject& Instantiate(GameObjectTemplate& gameObjectTemplate);
			void Destroy(GameObject& gameObject);

		private:
			GameObjectWorld* const _gameObjectWorld;

			PREWorld();
			~PREWorld();

			void Initialize();
			void Update();
			void Shutdown();
		};
	} // namespace Core
} // namespace PRE