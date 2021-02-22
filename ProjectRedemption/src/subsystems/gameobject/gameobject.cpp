#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <subsystems/gameobject/gameobjectworld.h>
#include <subsystems/gameobject/gameobject.h>
#include <subsystems/gameobject/gameobjectcomponent.h>

namespace GameObjectSubsystem
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
}