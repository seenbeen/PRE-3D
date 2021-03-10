#include <core/subsystems/rendering/preskeleton.h>

#include <string>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		using std::string;
		
		using PRE::RenderingModule::RenderSkeleton;

		void PRESkeleton::SetBoneLocalMatrix(
			const string bone,
			const glm::mat4& localMatrix
		)
		{
			_skeleton.SetBoneLocalMatrix(bone, localMatrix);
		}

		PRESkeleton::PRESkeleton(RenderSkeleton& skeleton)
			:
			_skeleton(skeleton) {}

		PRESkeleton::~PRESkeleton() {}
	} // namespace Core
} // namespace PRE