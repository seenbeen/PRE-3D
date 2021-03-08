#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class RenderAnimation;
		class RenderAnimationBoneConfig;

		using std::pair;
		using std::string;
		using std::unordered_map;
		using std::vector;

		class RenderAnimationBone
		{
			RenderAnimationBone& operator=(const RenderAnimationBone&) = delete;
			RenderAnimationBone(const RenderAnimationBone&) = delete;

			friend class RenderAnimation;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderAnimationBone;

			private:
				static Impl& MakeImpl(const RenderAnimationBoneConfig& animationBoneConfig);

				const vector<RenderAnimationBone*> children;

				// TODO: Can optimize for locality by storing current frame index
				//       for position, rotation, scale keyframes
				const vector<pair<float, glm::vec3>> positionKeyFrames;
				const vector<pair<float, glm::fquat>> rotationKeyFrames;
				const vector<pair<float, glm::vec3>> scaleKeyFrames;

				Impl(
					vector<RenderAnimationBone*>& children,
					vector<pair<float, glm::vec3>>& positionKeyFrames,
					vector<pair<float, glm::fquat>>& rotationKeyFrames,
					vector<pair<float, glm::vec3>>& scaleKeyFrames
				);
				~Impl();
			};
		public:
			const string name; // lambdas needa capture this to sort .-.

		private:
			static void GetBlendedStateAt(
				const RenderAnimationBone& a,
				const RenderAnimationBone& b,
				float timeA,
				float timeB,
				float blendFactor,
				const glm::mat4& parentMatrix,
				unordered_map<string, glm::mat4>& result
			);

			Impl& _impl;

			RenderAnimationBone(const RenderAnimationBoneConfig& animationBoneConfig);
			~RenderAnimationBone();

			void GetStateAt(
				float time,
				const glm::mat4& parentMatrix,
				unordered_map<string, glm::mat4>& result
			) const;
		};
	} // namespace RenderingModule
} // namespace PRE