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

		void PRESkeleton::SetBoneLocalMatrix(const string bone, const glm::mat4& localMatrix)
		{
			_skeleton.SetBoneLocalMatrix(bone, localMatrix);
		}
	} // namespace Core
} // namespace PRE