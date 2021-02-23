#include <modules/gameobject/gameobject.h>

#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <modules/gameobject/gameobjectworld.h>
#include <modules/gameobject/gameobjectcomponent.h>

namespace PRE
{
	namespace GameObjectModule
	{
		GameObject& GameObject::Instantiate(GameObjectTemplate& gameObjectTemplate)
		{
			return _gameObjectWorld.Instantiate(gameObjectTemplate);
		}

		void GameObject::Destroy(GameObject& gameObject)
		{
			_gameObjectWorld.Destroy(gameObject);
		}

		GameObject::GameObject(GameObjectWorld& gameObjectWorld)
			: _gameObjectWorld(gameObjectWorld)
		{

		}

		GameObject::~GameObject()
		{
			for (auto it = _gameObjectComponents.begin(); it != _gameObjectComponents.end(); ++it)
			{
				delete it->second;
			}
		}

		void GameObject::Start()
		{
			for (auto it = _gameObjectComponents.begin(); it != _gameObjectComponents.end(); ++it)
			{
				it->second->OnStart();
			}
		}

		void GameObject::Update()
		{
			for (auto it = _gameObjectComponents.begin(); it != _gameObjectComponents.end(); ++it)
			{
				it->second->OnUpdate();
			}
		}

		void GameObject::Destroy()
		{
			for (auto it = _gameObjectComponents.begin(); it != _gameObjectComponents.end(); ++it)
			{
				it->second->OnDestroy();
			}
		}
	} // namespace GameObjectSubsystem
} // namespace PRE