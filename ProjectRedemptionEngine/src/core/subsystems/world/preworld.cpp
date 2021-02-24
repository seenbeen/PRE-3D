#include <core/subsystems/world/preworld.h>

#include <include/modules/gameobject.h>

namespace PRE
{
	namespace Core
	{
		GameObject& PREWorld::Instantiate(GameObjectTemplate& gameObjectTemplate)
		{
			return _gameObjectWorld->Instantiate(gameObjectTemplate);
		}

		void PREWorld::Destroy(GameObject& gameObject)
		{
			_gameObjectWorld->Destroy(gameObject);
		}

		PREWorld::PREWorld()
			: _gameObjectWorld(new GameObjectWorld()) {}

		PREWorld::~PREWorld()
		{
			delete _gameObjectWorld;
		}

		void PREWorld::Initialize() {}

		void PREWorld::Update()
		{
			_gameObjectWorld->Update();
		}

		void PREWorld::Shutdown()
		{
			_gameObjectWorld->Shutdown();
		}
	}
}