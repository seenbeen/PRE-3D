#include <core/subsystems/rendering/preskeleton.h>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using PRE::RenderingModule::RenderSkeleton;

		PRESkeleton::PRESkeleton(RenderSkeleton& skeleton)
			:
			_skeleton(skeleton) {}

		PRESkeleton::~PRESkeleton() {}
	} // namespace Core
} // namespace PRE