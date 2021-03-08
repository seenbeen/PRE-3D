#include <modules/rendering/animation/renderanimationboneconfig.h>

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		RenderAnimationBoneConfig::KeyFrame::KeyFrame(
			float time,
			const KeyFrameKind& kind,
			const glm::vec3& vectorData
		)
			:
			time(time),
			kind(kind),
			vectorData(vectorData),
			quaternionData() {}

		RenderAnimationBoneConfig::KeyFrame::KeyFrame(
			float time,
			const glm::fquat& quaternionData
		)
			:
			time(time),
			kind(KeyFrameKind::ROTATION),
			vectorData(),
			quaternionData(quaternionData) {}

		RenderAnimationBoneConfig::RenderAnimationBoneConfig(const string& name)
			:
			_name(name) {}

		RenderAnimationBoneConfig::~RenderAnimationBoneConfig()
		{
			for (auto it = _children.begin(); it != _children.end(); ++it)
			{
				delete *it;
			}
			for (auto it = _keyFrames.begin(); it != _keyFrames.end(); ++it)
			{
				delete* it;
			}
		}

		void RenderAnimationBoneConfig::AddChild(const RenderAnimationBoneConfig& child)
		{
#ifdef __PRE_DEBUG__
			for (auto it = _children.begin(); it != _children.end(); ++it)
			{
				if ((*it)->_name == child._name)
				{
					throw "Duplicate bone name encountered";
				}
			}
#endif
			_children.push_back(&child);
		}

		void RenderAnimationBoneConfig::AddKeyFrame(
			float time,
			const KeyFrameKind& kind,
			const glm::vec3& vectorData
		)
		{
#ifdef __PRE_DEBUG__
			if (kind == KeyFrameKind::ROTATION)
			{
				throw "Illegal KeyFrameKind passed into vector KeyFrame";
			}
#endif

			_keyFrames.push_back(new KeyFrame(time, kind, vectorData));
		}

		void RenderAnimationBoneConfig::AddKeyFrame(
			float time,
			const glm::fquat& quaternionData
		)
		{
			_keyFrames.push_back(new KeyFrame(time, quaternionData));
		}
	} // namespace RenderingModule
} // namespace PRE