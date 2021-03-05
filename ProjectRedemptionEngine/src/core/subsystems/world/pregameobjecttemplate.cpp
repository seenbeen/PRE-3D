#include <core/subsystems/world/pregameobjecttemplate.h>

#include <core/preapplicationcontext.h>

#include <core/components/pretransformcomponent.h>

#include <core/subsystems/rendering/prerendering.h>

namespace PRE
{
	namespace Core
	{
		void PREGameObjectTemplate::OnInstantiate()
		{
			AddPREComponent<PRETransformComponent>();
			OnInstantiateTemplate();
		}

		PRERendering& PREGameObjectTemplate::GetRendering()
		{
			return _preApplicationContext->rendering;
		}
	} // namespace Core
} // namespace PRE