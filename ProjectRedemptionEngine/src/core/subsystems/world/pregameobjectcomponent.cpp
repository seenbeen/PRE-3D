#include <core/subsystems/world/pregameobjectcomponent.h>

#include <modules/gameobject/gameobjectcomponent.h>

#include <core/preapplicationcontext.h>

#include <core/subsystems/input/preinput.h>
#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/time/pretime.h>
#include <core/subsystems/world/preworld.h>

namespace PRE
{
	namespace Core
	{
		PREInput& PREGameObjectComponent::GetInput()
		{
			return _preApplicationContext->input;
		}

		PRERendering& PREGameObjectComponent::GetRendering()
		{
			return _preApplicationContext->rendering;
		}

		PRETime& PREGameObjectComponent::GetTime()
		{
			return _preApplicationContext->time;
		}

		PREWorld& PREGameObjectComponent::GetWorld()
		{
			return _preApplicationContext->world;
		}

		PREAssetManager& PREGameObjectComponent::GetAssetManager()
		{
			return _preApplicationContext->assetManager;
		}

		void PREGameObjectComponent::Quit()
		{
			_preApplicationContext->Quit();
		}
	} // namespace Core
} // namespace PRE