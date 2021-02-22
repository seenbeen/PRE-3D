#pragma once
#include <subsystems/gameobject/gameobject.h>

namespace GameObjectSubsystem
{
	class GameObjectWorld;
	class GameObjectComponent;

	class GameObjectTemplate
	{
		GameObjectTemplate& operator=(const GameObjectTemplate&) = delete;
		GameObjectTemplate(const GameObjectTemplate&) = delete;

		friend class GameObjectWorld;
		friend class GameObject;

	protected:
		GameObjectTemplate() = default;

		template<class TGameObjectComponent>
		TGameObjectComponent& AddComponent()
		{
			return _currentObject->AddComponent<TGameObjectComponent>();
		}

		virtual void Instantiate() = 0;

	private:
		GameObject* _currentObject = nullptr;

		/**
		 * Produces a game object from this template.
		 **/
		GameObject* InstantiateGameObject(GameObjectWorld&);
	};
}