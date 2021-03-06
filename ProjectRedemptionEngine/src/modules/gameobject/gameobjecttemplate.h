#pragma once
#include <modules/gameobject/gameobject.h>

namespace PRE
{
	namespace GameObjectModule
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
			GameObjectTemplate();

			template<class TGameObjectComponent>
			TGameObjectComponent* AddComponent()
			{
				return _currentGameObject->AddComponent<TGameObjectComponent>();
			}

			template<class TGameObjectComponent>
			TGameObjectComponent* GetComponent()
			{
				return _currentGameObject->GetComponent<TGameObjectComponent>();
			}

			GameObject& Instantiate(GameObjectTemplate& gameObjectTemplate);

			virtual void OnInstantiate() = 0;


		private:
			GameObjectWorld* _currentGameObjectWorld;
			GameObject* _currentGameObject;

			/**
				* Produces a game object from this template.
				**/
			GameObject& InstantiateGameObject(GameObjectWorld& gameObjectWorld);
		};
	} // namespace GameObjectSubsystem
} // namespace PRE