#include <core/subsystems/rendering/preboneconfig.h>

#include <string>

#include <glm/glm.hpp>

#include <modules/rendering/model/renderskeleton.h>

namespace PRE
{
	namespace Core
	{
		PREBoneConfig::PREBoneConfig(
			unsigned int id,
			const string& name,
			const glm::mat4& bindPos,
			const glm::mat4& startingTransform
		)
			:
			_boneConfig(id, name, bindPos, startingTransform) {}

		void PREBoneConfig::AddChild(const PREBoneConfig& child)
		{
			_boneConfig.AddChild(child._boneConfig);
		}
	} // namespace Core
} // namespace PRE