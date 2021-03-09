#pragma once
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace PRE
{
	namespace AnimationModule
	{
		class AnimationConfig;
		class AnimationChannelConfig;
		class AnimationChannel;

		using std::string;
		using std::unordered_map;

		class Animation
		{
			Animation& operator=(const Animation&) = delete;
			Animation(const Animation&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class Animation;

			private:
				static Impl& MakeImpl(
					float ticksPerSecond,
					float duration,
					const AnimationConfig& animationConfig
				);

				const float ticksPerSecond;
				const float duration;

				unordered_map<string, AnimationChannel*> channels;

				Impl(
					float ticksPerSecond,
					float duration,
					unordered_map<string, AnimationChannel*>& channels
				);
				~Impl();
			};

		public:
			Animation(
				float ticksPerSecond,
				float duration,
				AnimationConfig& animationConfig
			);
			~Animation();

			static void GetBlendedStateAt(
				const Animation& a,
				const Animation& b,
				float timeA,
				float timeB,
				float blendFactor,
				unordered_map<string, glm::mat4>& result
			);

			float GetDuration() const;
			void GetStateAt(
				float time,
				unordered_map<string, glm::mat4>& result
			) const;

		private:
			Impl& _impl;
		};
	} // namespace AnimationModule
} // namespace PRE