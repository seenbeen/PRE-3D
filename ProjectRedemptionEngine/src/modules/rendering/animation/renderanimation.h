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

				Impl(
					float ticksPerSecond,
					float duration,
					RenderAnimationBone& rootBone
				);
				~Impl();
			};

		public:
			static void GetBlendedStateAt(
				const RenderAnimation& a,
				const RenderAnimation& b,
				float timeA,
				float timeB,
				float blendFactor,
				unordered_map<string, glm::mat4>& result
			);

			float GetDuration() const;
			void GetStateAt(float time, unordered_map<string, glm::mat4>& result) const;

		private:
			Impl& _impl;

			RenderAnimation(float ticksPerSecond, float duration, RenderAnimationBoneConfig& rootBoneConfig);
			~RenderAnimation();
		};
	} // namespace RenderingModule
} // namespace PRE