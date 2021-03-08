#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderAnimationBone;

		using std::string;
		using std::vector;

		class RenderAnimationBoneConfig
		{
			RenderAnimationBoneConfig& operator=(const RenderAnimationBoneConfig&) = delete;
			RenderAnimationBoneConfig(const RenderAnimationBoneConfig&) = delete;

			friend class RenderAnimationBone;

		public:
			enum class KeyFrameKind { POSITION, ROTATION, SCALE };

		private:
			struct KeyFrame
			{
				const float time;
				const KeyFrameKind kind;
				const glm::vec3 vectorData;
				const glm::fquat quaternionData;

				KeyFrame(
					float time,
					const KeyFrameKind& kind,
					const glm::vec3& vectorData
				);

				KeyFrame(
					float time,
					const glm::fquat& quaternionData
				);
			};

			const string _name;
			vector<const RenderAnimationBoneConfig*> _children;
			vector<KeyFrame*> _keyFrames;

		public:
			RenderAnimationBoneConfig(const string& name);
			~RenderAnimationBoneConfig();

			void AddChild(const RenderAnimationBoneConfig& child);
			void AddKeyFrame(
				float time,
				const KeyFrameKind& kind,
				const glm::vec3& vectorData
			);
			void AddKeyFrame(
				float time,
				const glm::fquat& quaternionData
			);
		};
	} // namespace RenderingModule
} // namespace PRE