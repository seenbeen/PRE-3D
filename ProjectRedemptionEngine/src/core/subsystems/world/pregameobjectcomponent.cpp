#include <core/subsystems/world/pregameobjectcomponent.h>

#include <modules/gameobject/gameobjectcomponent.h>

#include <core/preapplicationcontext.h>

#include <core/subsystems/rendering/prerendering.h>
#include <core/subsystems/time/pretime.h>
#include <core/subsystems/world/preworld.h>

namespace PRE
{
	namespace Core
	{
		PRERendering* PREGameObjectComponent::GetRendering()
		{
			return _preApplicationContext->rendering;
		}

		PRETime* PREGameObjectComponent::GetTime()
		{
			return _preApplicationContext->time;
		}

		PREWorld* PREGameObjectComponent::GetWorld()
		{
			return _preApplicationContext->world;
		}

		void PREGameObjectComponent::Quit()
		{
			_preApplicationContext->Quit();
		}
	} // namespace Core
} // namespace PRE