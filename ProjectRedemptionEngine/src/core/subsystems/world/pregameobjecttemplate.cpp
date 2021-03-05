#include <core/subsystems/world/pregameobjecttemplate.h>

#include <core/preapplicationcontext.h>

#include <core/components/pretransformcomponent.h>

namespace PRE
{
	namespace Core
	{
		void PREGameObjectTemplate::OnInstantiate()
		{
			AddPREComponent<PRETransformComponent>();
			OnInstantiateTemplate();
		}
	} // namespace Core
} // namespace PRE