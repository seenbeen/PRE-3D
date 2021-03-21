#include <core/subsystems/world/pregameobjecttemplate.h>

#include <core/preapplicationcontext.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/world/preworld.h>

namespace PRE
{
	namespace Core
	{
		using PRE::GameObjectModule::GameObject;

		void PREGameObjectTemplate::OnInstantiate()
		{
			AddPREComponent<PRETransformComponent>();
			OnInstantiateTemplate();
		}

		GameObject& PREGameObjectTemplate::Instantiate(PREGameObjectTemplate& gameObjectTemplate)
		{
			return _preApplicationContext->world.Instantiate(gameObjectTemplate);
		}
	} // namespace Core
} // namespace PRE