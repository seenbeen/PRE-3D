#include <subsystems/gameobject/gameobjectworld.h>
#include <subsystems/gameobject/gameobjecttemplate.h>
#include <subsystems/gameobject/gameobject.h>

namespace PRE
{
	namespace GameObjectSubsystem
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