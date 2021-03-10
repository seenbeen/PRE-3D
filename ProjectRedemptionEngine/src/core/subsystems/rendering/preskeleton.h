#pragma once
#include <string>

#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using std::string;

		using PRE::RenderingModule::RenderSkeleton;

		class PRESkeleton
		{
			PRESkeleton& operator=(const PRESkeleton&) = delete;
			PRESkeleton(const PRESkeleton&) = delete;

			friend class PRERendering;
		
		public:
			void SetBoneLocalMatrix(const string bone, const glm::mat4& localMatrix);

		private:
			RenderSkeleton& _skeleton;

			PRESkeleton(RenderSkeleton& skeleton);
			~PRESkeleton();
		};
	} // namespace Core
} // namespace PRE