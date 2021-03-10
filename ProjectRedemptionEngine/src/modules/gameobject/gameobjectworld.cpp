#include <modules/gameobject/gameobjectworld.h>

#include <unordered_set>

#include <modules/gameobject/gameobjecttemplate.h>
#include <modules/gameobject/gameobject.h>

namespace PRE
{
	namespace GameObjectModule
	{
		GameObjectWorld::~GameObjectWorld()
		{
			for (auto it = _startingObjects.begin(); it != _startingObjects.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _runningObjects.begin(); it != _runningObjects.end(); ++it)
			{
				delete *it;
			}
			// note: destroyed objects were cleared along with running objects
		}

		GameObject& GameObjectWorld::Instantiate(GameObjectTemplate& gameObjectTemplate)
		{
			auto& gameObject = gameObjectTemplate.InstantiateGameObject(*this);
			_startingObjects.insert(&gameObject);
			return gameObject;
		}

		void GameObjectWorld::Update()
		{
			// incase objects are mutated during frame, a container swap is done
			// to defer to following frame and prevent mutation-during-iteration
			unordered_set<GameObject*> tempStartingObjects = _startingObjects;
			_startingObjects.clear();

			// first, all objects starting before this frame are started
			for (auto it = tempStartingObjects.begin(); it != tempStartingObjects.end(); ++it)
			{
				auto pGameObject = *it;
				pGameObject->Start();
				_runningObjects.insert(pGameObject);
			}

			// next, all objects that exist in the context of this frame are ticked
			for (auto it = _runningObjects.begin(); it != _runningObjects.end(); ++it)
			{
				auto pGameObject = *it;
				pGameObject->Update();
			}

			// finally, all objects destroyed this frame are blown-up
			// if Destroy is called within Destroy, it is treated in the following frame
			unordered_set<GameObject*> tempDestroyedObjects = _destroyedObjects;
			_destroyedObjects.clear();

			for (auto it = tempDestroyedObjects.begin(); it != tempDestroyedObjects.end(); ++it)
			{
				auto pGameObject = *it;
				_runningObjects.erase(pGameObject);
				pGameObject->Destroy();
				delete pGameObject;
			}
		}

		void GameObjectWorld::Destroy(GameObject& gameObject)
		{
			auto pGameObject = &gameObject;
			auto it = _startingObjects.find(pGameObject);
			// Prevent un-started object from starting
			if (it != _startingObjects.end()) {
				_startingObjects.erase(it);
				return;
			}

#ifdef __PRE_DEBUG__
			// Only allow destruction of what exists
			it = _runningObjects.find(pGameObject);
			if (it == _runningObjects.end()) {
				throw "GameObject does not belong to GameObjectWorld.";
			}
#endif

			_destroyedObjects.insert(pGameObject);
		}

		void GameObjectWorld::Shutdown()
		{
			for (auto it = _startingObjects.begin(); it != _startingObjects.end(); ++it)
			{
				delete *it;
			}
			_startingObjects.clear();
			for (auto it = _runningObjects.begin(); it != _runningObjects.end(); ++it)
			{
				(*it)->Destroy();
				delete *it;
			}
			_runningObjects.clear();
			// note: destroyed objects were cleared along with running objects
		}
	} // namespace GameObjectSubsystem
} // namespace PRE