#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderAnimationBoneConfig;
		class RenderAnimationBone;

		using std::string;
		using std::unordered_map;

		class RenderAnimation
		{
			RenderAnimation& operator=(const RenderAnimation&) = delete;
			RenderAnimation(const RenderAnimation&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderAnimation;

			private:
				static Impl& MakeImpl(
					float ticksPerSecond,
					float duration,
					RenderAnimationBoneConfig& rootBoneConfig
				);

				const float ticksPerSecond;
				const float duration;

				RenderAnimationBone& rootBone;
				const unordered_map<string, RenderAnimationBone*> boneMap;

				Impl(
					float ticksPerSecond,
					float duration,
					RenderAnimationBone& rootBone,
					unordered_map<string, RenderAnimationBone*>& boneMap
				);
				~Impl();
			};

		public:
			void SetTime(float time);
			float GetDuration();
			const glm::mat4& GetCurrentBoneMatrix(const string& boneName) const;

		private:
			Impl& _impl;

			RenderAnimation(float ticksPerSecond, float duration, RenderAnimationBoneConfig& rootBoneConfig);
			~RenderAnimation();
		};
	} // namespace RenderingModule
} // namespace PRE