#include <modules/gameobject/gameobjecttemplate.h>

#include <modules/gameobject/gameobjectworld.h>
#include <modules/gameobject/gameobject.h>

namespace PRE
{
	namespace GameObjectModule
	{
		GameObject* GameObjectTemplate::InstantiateGameObject(GameObjectWorld& gameObjectWorld)
		{
			_currentObject = new GameObject(gameObjectWorld);
			Instantiate();
			auto temp = _currentObject;
			_currentObject = nullptr;
			return temp;
		}
	} // namespace GameObjectSubsystem
} // namespace PRE