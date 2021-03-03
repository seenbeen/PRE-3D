#include <core/subsystems/world/preworld.h>

#include <core/subsystems/world/pregameobjecttemplate.h>

#include <include/modules/gameobject.h>

namespace PRE
{
	namespace Core
	{
		GameObject& PREWorld::Instantiate(PREGameObjectTemplate& preGameObjectTemplate)
		{
			preGameObjectTemplate._preApplicationContext = &_preApplicationContext;
			return _gameObjectWorld.Instantiate(preGameObjectTemplate);
		}

		void PREWorld::Destroy(GameObject& gameObject)
		{
			_gameObjectWorld.Destroy(gameObject);
		}

		PREWorld& PREWorld::MakePREWorld(
			const PREWorldConfig& worldConfig,
			PREApplicationContext& applicationContext
		)
		{
			auto& gameObjectWorld = *(new GameObjectWorld());
			return *(new PREWorld(applicationContext, gameObjectWorld));
		}

		PREWorld::PREWorld(
			PREApplicationContext& preApplicationContext,
			GameObjectWorld& gameObjectWorld
		)
			:
			_preApplicationContext(preApplicationContext),
			_gameObjectWorld(gameObjectWorld) {}

		PREWorld::~PREWorld()
		{
			delete &_gameObjectWorld;
		}

		void PREWorld::Initialize() {}

		void PREWorld::Update()
		{
			_gameObjectWorld.Update();
		}

		void PREWorld::Shutdown()
		{
			_gameObjectWorld.Shutdown();
		}
	}
}