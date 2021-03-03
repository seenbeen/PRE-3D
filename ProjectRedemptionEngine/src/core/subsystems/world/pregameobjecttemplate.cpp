#include <core/subsystems/world/pregameobjecttemplate.h>
#include <core/components/pretransformcomponent.h>

namespace PRE
{
	namespace Core
	{
		void PREGameObjectTemplate::OnInstantiate()
		{
			AddPREComponent<PRETransformComponent>();
			Instantiate();
		}
	} // namespace Core
} // namespace PRE