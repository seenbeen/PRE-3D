#include <modules/gameobject/gameobjectcomponent.h>

namespace PRE
{
	namespace GameObjectModule
	{
		GameObject& GameObjectComponent::gameObject()
		{
			return *_pGameObject;
		}

		void GameObjectComponent::OnStart() {}

		void GameObjectComponent::OnUpdate() {}

		void GameObjectComponent::OnDestroy() {}
	} // namespace GameObjectSubsystem
} // namespace PRE