#pragma once
#include <vector>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using std::string;
		using std::vector;

		using PRE::RenderingModule::RenderSkeleton;

		class PRESkeleton
		{
			PRESkeleton& operator=(const PRESkeleton&) = delete;
			PRESkeleton(const PRESkeleton&) = delete;

			friend class PRERendering;
			friend class PREAnimatorComponent;

		private:
			RenderSkeleton& _skeleton;

			PRESkeleton(RenderSkeleton& skeleton);
			~PRESkeleton();

			void SetBoneLocalMatrix(const string bone, const glm::mat4& localMatrix);
		};
	} // namespace Core
} // namespace PRE