#pragma once
#include <string>

#include <glm/glm.hpp>

#include <modules/rendering/model/renderskeleton.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using std::string;

		using PRE::RenderingModule::RenderSkeleton;

		class PREBoneConfig
		{
			PREBoneConfig& operator=(const PREBoneConfig&) = delete;
			PREBoneConfig(const PREBoneConfig&) = delete;
		
			friend class PRERendering;

		public:
			PREBoneConfig(
				unsigned int id,
				const string& name,
				const glm::mat4& bindPos,
				const glm::mat4& startingTransform
			);

			void AddChild(const PREBoneConfig& child);

		private:
			RenderSkeleton::BoneConfig _boneConfig;
		};
	} // namespace Core
} // namespace PRE