#pragma once
#include <typeindex>
#include <unordered_map>

#include <modules/gameobject/gameobjectcomponent.h>

using std::unordered_map;
using std::type_index;
namespace PRE
{
	namespace GameObjectModule
	{
		class GameObjectWorld;
		class GameObjectTemplate;

		class GameObject
		{
			GameObject& operator=(const GameObject&) = delete;
			GameObject(const GameObject&) = delete;

			friend class GameObjectWorld;
			friend class GameObjectTemplate;
			friend class GameObjectComponent;

		public:
			template<class TGameObjectComponent>
			TGameObjectComponent* GetComponent()
			{
				auto& typeId = typeid(TGameObjectComponent);
				auto existingElement = _gameObjectComponents.find(typeId);
				if (existingElement == _gameObjectComponents.end())
				{
					return nullptr;
				}
				return static_cast<TGameObjectComponent*>(existingElement->second);
			}

			GameObject& Instantiate(GameObjectTemplate& gameObjectTemplate);
			void Destroy(GameObject& gameObject);

		private:
			unordered_map<type_index, GameObjectComponent*> _gameObjectComponents;
			GameObjectWorld& _gameObjectWorld;

			GameObject(GameObjectWorld& gameObjectWorld);
			~GameObject();

			template<class TGameObjectComponent>
			TGameObjectComponent* AddComponent()
			{
				auto& typeId = typeid(TGameObjectComponent);
				if (_gameObjectComponents.find(typeId) != _gameObjectComponents.end())
				{
					throw "GameObjectComponent already exists.";
				}

				auto newComponent = new TGameObjectComponent();
				newComponent->_pGameObject = this;
				_gameObjectComponents[typeId] = newComponent;
				return newComponent;
			}

			void Start();
			void Update();
			void Destroy();
		};
	} // namespace GameObjectSubsystem
} // namespace PRE