#pragma once
#include <string>
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

				// TODO: Can optimize for locality by storing current frame index
				//       for position, rotation, scale keyframes
				vector<pair<float, glm::vec3>> positionKeyFrames;
				vector<pair<float, glm::fquat>> rotationKeyFrames;
				vector<pair<float, glm::vec3>> scaleKeyFrames;


				glm::mat4 parentMatrix;
				glm::mat4 currentMatrix;

				Impl(
					vector<pair<float, glm::vec3>>& positionKeyFrames,
					vector<pair<float, glm::fquat>>& rotationKeyFrames,
					vector<pair<float, glm::vec3>>& scaleKeyFrames
				);
				~Impl();
			};

		private:
			const string _name;			
			vector<RenderAnimationBone*> _children;

			Impl& _impl;

			RenderAnimationBone(const RenderAnimationBoneConfig& animationBoneConfig);
			~RenderAnimationBone();

			void SetTime(float time);
			const glm::mat4& GetCurrentMatrix() const;
		};
	} // namespace RenderingModule
} // namespace PRE