#include <modules/gameobject/gameobjecttemplate.h>

#include <modules/gameobject/gameobjectworld.h>
#include <modules/gameobject/gameobject.h>

namespace PRE
{
	namespace GameObjectModule
	{
		GameObjectTemplate::GameObjectTemplate()
		{
			_currentGameObjectWorld = nullptr;
			_currentGameObject = nullptr;
		}

		GameObject& GameObjectTemplate::Instantiate(GameObjectTemplate& gameObjectTemplate)
		{
			return _currentGameObjectWorld->Instantiate(gameObjectTemplate);
		}

		GameObject& GameObjectTemplate::InstantiateGameObject(GameObjectWorld& gameObjectWorld)
		{
			_currentGameObjectWorld = &gameObjectWorld;
			_currentGameObject = new GameObject(gameObjectWorld);
			OnInstantiate();
			auto temp = _currentGameObject;
			_currentGameObject = nullptr;
			return *temp;
		}
	} // namespace GameObjectSubsystem
} // namespace PRE