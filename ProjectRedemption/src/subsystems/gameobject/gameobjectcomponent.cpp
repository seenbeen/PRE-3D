#include <subsystems/gameobject/gameobjectcomponent.h>

namespace GameObjectSubsystem
{
	GameObject& GameObjectComponent::gameObject()
	{
		return *_pGameObject;
	}
}