#pragma once
#include <unordered_set>

using std::unordered_set;

namespace PRE
{
	namespace GameObjectModule
	{
		class GameObjectTemplate;
		class GameObject;

		class GameObjectWorld
		{
			GameObjectWorld& operator=(const GameObjectWorld&) = delete;
			GameObjectWorld(const GameObjectWorld&) = delete;

		public:
			GameObjectWorld() = default;
			~GameObjectWorld();

			GameObject& Instantiate(GameObjectTemplate& gameObjectTemplate);
			void Update();
			void Destroy(GameObject& gameObject);

			void Shutdown();
		private:
			unordered_set<GameObject*> _startingObjects;
			unordered_set<GameObject*> _runningObjects;
			unordered_set<GameObject*> _destroyedObjects;
		};
	} // namespace GameObjectSubsystem
} // namespace PRE