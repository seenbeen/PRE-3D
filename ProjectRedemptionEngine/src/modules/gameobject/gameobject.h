#pragma once
#include <typeindex>
#include <unordered_map>

#include <modules/gameobject/gameobjectcomponent.h>

namespace PRE
{
	namespace GameObjectModule
	{
		class GameObjectWorld;
		class GameObjectTemplate;
		
		using std::unordered_map;
		using std::type_index;
		
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

#ifdef __PRE_DEBUG__
				if (_gameObjectComponents.find(typeId) != _gameObjectComponents.end())
				{
					throw "GameObjectComponent already exists.";
				}
#endif

				auto pNewComponent = new TGameObjectComponent();
				pNewComponent->_pGameObject = this;
				_gameObjectComponents[typeId] = pNewComponent;
				return pNewComponent;
			}

			void Start();
			void Update();
			void Destroy();
		};
	} // namespace GameObjectSubsystem
} // namespace PRE